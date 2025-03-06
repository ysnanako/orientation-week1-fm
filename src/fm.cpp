#include "fm.h"
using namespace std;

void FM::Run()
{
    auto start = chrono::high_resolution_clock::now();
    Initial();
    for(int i = 0; i < iteration; ++i)
    {
        node *target = MaxGainNode(group_type::A);
        while(target != NULL)
        {
            MoveNode(target);
            UpdateGain(target);
            if(size_A > total_size * size_rate)
                target = MaxGainNode(group_type::A);
            else
                target = MaxGainNode(group_type::B);
        }
        cut_size = CutSize();
        ResetLock();
        cout << "\n-------------------------------------------------------------------------\n";
        for(size_t i = 0; i < node_vec.size(); ++i)
        {
            cout << i << " ";
            SetBucket(node_vec[i]);
        }
    }
    auto end = chrono::high_resolution_clock::now();
    elapsed = end - start;
}
void FM::Initial()
{
    for(size_t i = 0; i < node_vec.size(); ++i)
    {
        if(size_A < total_size * size_rate)
        {
            node_vec[i]->group = group_type::A;
            size_A += node_vec[i]->size;
        }  
        else
            node_vec[i]->group = group_type::B;
    }
    for(size_t i = 0; i < net_vec.size(); ++i)
    {
        SetGain(net_vec[i]);
    }
    bucket_A.resize(2 * offset + 1);
    bucket_B.resize(2 * offset + 1);
    max_A = offset;
    max_B = offset;
    for(size_t i = 0; i < node_vec.size(); ++i)
    {
        SetBucket(node_vec[i]);
    }
    init = true;
}
void FM::MoveNode(node *target)
{
    if(target->group == group_type::A)
    {
        target->group = group_type::B;
        size_A -= target->size;
    }
    else
    {
        target->group = group_type::A;
        size_A += target->size;
    }
    target->lock = true;
}
void FM::UpdateGain(node *target)
{
    for(size_t i = 0; i < target->nets.size(); ++i)
    {
        SetGain(net_vec[target->nets[i]]);
        for(size_t j = 0; j < net_vec[target->nets[i]]->nodes.size(); ++j)
        {
            SetBucket(node_vec[net_vec[target->nets[i]]->nodes[j]]);
        }
    }
}
void FM::SetGain(net *net_ptr)
{
    // dontcare net
    if(net_ptr->nodes.size() == 1)
        return;
    // set state
    int a = 0, b = 0;
    for(size_t i = 0; i < net_ptr->nodes.size(); ++i)
    {
        if(node_vec[net_ptr->nodes[i]]->group == group_type::A)
            a++;
        else
            b++;
    }
    if(a == 0 || b == 0)
        net_ptr->state = state_type::united;
    else if(a > 1 && b > 1)
        net_ptr->state = state_type::dontcare;
    else if(a == 1 && b == 1)
        net_ptr->state = state_type::separated;
    else if(a == 1)
        net_ptr->state = state_type::A;
    else if(b == 1)
        net_ptr->state = state_type::B;
    // reset diff
    int diff = 0;
    if(net_ptr->pre_state == state_type::united)
        diff -= -1;
    else if(net_ptr->pre_state == state_type::separated)
        diff -= 1;
    else if(net_ptr->pre_state == state_type::A)
    {
        for(size_t i = 0; i < net_ptr->nodes.size(); ++i)
        {
            if(node_vec[net_ptr->nodes[i]]->group == group_type::A)
                node_vec[net_ptr->nodes[i]]->NG -= 1;    
        }
    }
    else if(net_ptr->pre_state == state_type::B)
    {
        for(size_t i = 0; i < net_ptr->nodes.size(); ++i)
        {
            if(node_vec[net_ptr->nodes[i]]->group == group_type::B)
                node_vec[net_ptr->nodes[i]]->NG -= 1;
        }
    }
    // set diff
    if(net_ptr->state == state_type::united)
        diff += -1;
    else if(net_ptr->state == state_type::separated)
        diff += 1;
    else if(net_ptr->state == state_type::A)
    {
        for(size_t i = 0; i < net_ptr->nodes.size(); ++i)
        {
            if(node_vec[net_ptr->nodes[i]]->group == group_type::A)
                node_vec[net_ptr->nodes[i]]->NG += 1;
        }
    }
    else if(net_ptr->state == state_type::B)
    {
        for(size_t i = 0; i < net_ptr->nodes.size(); ++i)
        {
            if(node_vec[net_ptr->nodes[i]]->group == group_type::B)
                node_vec[net_ptr->nodes[i]]->NG += 1;
        }
    }
    // set gain with diff
    if(diff != 0)
    {
        for(size_t i = 0; i < net_ptr->nodes.size(); ++i)
        {
            node_vec[net_ptr->nodes[i]]->NG += diff;
        }
    }
    // set pre_state
    net_ptr->pre_state = net_ptr->state;
}
void FM::SetBucket(node *node_ptr)
{
    vector<list<node *>> *bucket_ptr;
    int *max;
    if(node_ptr->group == group_type::A)
    {
        bucket_ptr = &bucket_A;
        max = &max_A;
    }
    else
    {
        bucket_ptr = &bucket_B;
        max = &max_B;
    }
    if(init) // update node
    {
        auto it = find((*bucket_ptr)[node_ptr->G + offset].begin(), (*bucket_ptr)[node_ptr->G + offset].end(), node_ptr);
        if(it != (*bucket_ptr)[node_ptr->G + offset].end())
            (*bucket_ptr)[node_ptr->G + offset].erase(it);
    }
    (*bucket_ptr)[node_ptr->NG + offset].push_back(node_ptr);
    node_ptr->G = node_ptr->NG;
    if((*max) < node_ptr->G)
        (*max) = node_ptr->G;
}
node *FM::MaxGainNode(group_type group)
{
    vector<list<node *>> *bucket_ptr;
    int *max;
    if(group == group_type::A)
    {
        bucket_ptr = &bucket_A;
        max = &max_A;
    }
    else
    {
        bucket_ptr = &bucket_B;
        max = &max_B;
    }
    while((*max) >= 0)
    {
        if((*bucket_ptr)[(*max) + offset].empty())
        {
            (*max)--;
            continue;
        }
        for(auto it = (*bucket_ptr)[(*max) + offset].begin(); it != (*bucket_ptr)[(*max) + offset].end(); ++it)
        {
            if((*it)->lock == false)
            {
                cout << (*max) << "/" << (*it)->G << " ";
                return *it;
            }
        }
        (*max)--;
    }
    return NULL;
}
int FM::CutSize()
{
    int cut = 0;
    for(size_t i = 0; i < net_vec.size(); ++i)
    {
        if(net_vec[i]->state != state_type::united)
            cut++;
    }
    return cut;
}
void FM::ResetLock()
{
    for(size_t i = 0; i < node_vec.size(); ++i)
    {
        node_vec[i]->lock = false;
    }
}
// void FM::ResetMax()
// {
//     for(auto it = bucket_A.begin(); it != bucket_A.end(); ++it)
//     {
//         if(max_A < it->first && !it->second.empty())
//             max_A = it->first;
//     }
//     for(auto it = bucket_B.begin(); it != bucket_B.end(); ++it)
//     {
//         if(max_B < it->first && !it->second.empty())
//             max_B = it->first;
//     }
// }