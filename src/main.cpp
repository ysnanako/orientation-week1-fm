#include "fm.h"
#include "parser.h"
using namespace std;

int main(int argc, char *argv[])
{  
    PARSER PARSER_obj;
    FM FM_obj;
    PARSER_obj.GetFilename(argc, argv);
    PARSER_obj.ReadNodeFile(FM_obj);
    PARSER_obj.ReadNetFile(FM_obj);
    FM_obj.Run();
    PARSER_obj.WriteFile(FM_obj);
    return EXIT_SUCCESS; 
}