#ifndef _PARSER_H_
#define _PARSER_H_

#include <bits/stdc++.h>
#include "fm.h"
using namespace std;

class PARSER
{
    public:
    string node_filename, net_filename, out_filename;
    void GetFilename(const int& argc, char *argv[]);
    void ReadNodeFile(FM &);
    void ReadNetFile(FM &);
    void WriteFile(FM &);
    private:
    ifstream fin;
    ofstream fout;
    void CreatNode(FM &, string);
    void SetNet(FM &, string);
};

#endif