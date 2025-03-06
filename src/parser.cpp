#include "parser.h"
using namespace std;

void PARSER::GetFilename(const int& argc, char *argv[])
{
    if(argc == 4)
    {
        this->node_filename = argv[1];
        this->net_filename = argv[2];
        this->out_filename = argv[3];
    }
    else
    {
        cerr << "Format: <circuit.nodes> <circuit.nets> [circuit.out]. \n";
        exit(EXIT_FAILURE);
    }
}
void PARSER::ReadNodeFile(FM &FM_obj) // read node
{
    fin.open(node_filename);
    if(!fin)
    {
        cerr << "Error: Input .nodes file failed. \n";
        exit(EXIT_FAILURE);
    }
    string line;
    while(getline(fin, line))
    {
        if(line.substr(0, 2) == "\to")
            CreatNode(FM_obj, line);
    }
    fin.close();
}
void PARSER::CreatNode(FM &FM_obj, string line)
{
    if(line.back() == 'l') // terminal node
        return;
    node *new_ptr = new node;
    stringstream ss(line);
    string s;
    while(ss >> s)
    {
        if(s.front() != 'o')
            new_ptr->size *= stoi(s);
    }
    FM_obj.total_size += new_ptr->size;
    FM_obj.node_vec.push_back(new_ptr);   
}
void PARSER::ReadNetFile(FM &FM_obj)
{
    fin.open(net_filename);
    if(!fin)
    {
        cerr << "Error: Input .nets file failed. \n";
        exit(EXIT_FAILURE);
    }
    string line;
    while(getline(fin, line))
    {
        if(line.substr(0, 9) == "NetDegree")
            FM_obj.net_vec.push_back(new net);
        else if(line.substr(0, 2) == "\to")
            SetNet(FM_obj, line);
    }
    fin.close();
}
void PARSER::SetNet(FM &FM_obj, string line)
{
    stringstream ss(line);
    string s;
    ss >> s;
    size_t index = stoul(s.erase(0, 1));
    if(index >= FM_obj.node_vec.size()) // terminal node
        return;
    FM_obj.net_vec.back()->nodes.push_back(index);
    FM_obj.node_vec[index]->nets.push_back(FM_obj.net_vec.size() - 1);
    if(FM_obj.offset < (int)FM_obj.node_vec[index]->nets.size()) // set offset(pmax)
        FM_obj.offset = (int)FM_obj.node_vec[index]->nets.size();
}
void PARSER::WriteFile(FM &FM_obj)
{
    fout.open(out_filename);
    if(!fout)
    {
        cerr << "Error: Output .out file failed. \n";
        exit(EXIT_FAILURE);
    }
    // for(size_t i = 0; i < FM_obj.node_vec.size(); ++i)
    // {
    //     fout << "o" << i << " ";
    //     if(FM_obj.node_vec[i]->group == group_type::A)
    //         fout << "A ";
    //     else
    //         fout << "B ";
    //     fout << FM_obj.node_vec[i]->gain << "\n";
    // }
    for(int i = 0; i < (int)FM_obj.bucket_A.size(); ++i)
    {
        cout << "bucketA gain" << i - FM_obj.offset << " " << FM_obj.bucket_A[i].size() << "\n";
    }
    for(int i = 0; i < (int)FM_obj.bucket_B.size(); ++i)
    {
        cout << "bucketB gain" << i - FM_obj.offset << " " << FM_obj.bucket_B[i].size() << "\n";
    }
    fout << "Cut_size: " << FM_obj.cut_size << "\n";
    fout << "G1_size: " << FM_obj.size_A << "\n";
    fout << "G2_size: " << FM_obj.total_size - FM_obj.size_A << "\n";
    fout << "Time: " << FM_obj.elapsed.count() << " s\n";
    fout << "G1: \n";
    for(size_t i = 0; i < FM_obj.node_vec.size(); ++i)
    {
        if(FM_obj.node_vec[i]->group == group_type::A)
            fout << "o" << i << " ";
    }
    fout << "\b;\nG2: \n";
    for(size_t i = 0; i < FM_obj.node_vec.size(); ++i)
    {
        if(FM_obj.node_vec[i]->group == group_type::B)
            fout << "o" << i << " ";
    }
    fout << "\b;\nCut_set: \n";
    for(size_t i = 0; i < FM_obj.net_vec.size(); ++i)
    {
        if(FM_obj.net_vec[i]->state != state_type::united)
            fout << "n" << i << " ";
    }
    fout << "\b;";
}