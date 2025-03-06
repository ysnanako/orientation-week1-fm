#ifndef _FM_H_
#define _FM_H_

#include <bits/stdc++.h>
using namespace std;

#define size_rate 0.4
#define iteration 2

enum class group_type{A, B};
enum class state_type{united, separated, A, B, dontcare};
struct node
{
    double size = 1;
    int G = 0, NG = 0; // gain
    vector<int> nets;
    group_type group;
    bool lock = false;
};
struct net
{
    vector<int> nodes;
    state_type state, pre_state = state_type::dontcare;
};
class FM
{
    public:
    chrono::duration<double> elapsed;
    vector<node *> node_vec;
    vector<net *> net_vec;
    double total_size = 0;
    double size_A = 0;
    void Run();
    int CutSize();
    int cut_size, offset = 0;
    int max_A = 0, max_B = 0;
    vector<list<node *>> bucket_A, bucket_B;
    private:
    bool init = false;
    void Initial();
    void MoveNode(node *);
    void UpdateGain(node *);
    void SetGain(net *);
    void SetBucket(node *);
    node *MaxGainNode(group_type);
    void ResetLock();
    // void ResetMax();
};

#endif