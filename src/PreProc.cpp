#include "PreProc.hh"
#include <sstream>
#include <iostream>


bool preProc(const char* fileName, std::istringstream &stream){

    std::string cmd= "cpp -P ";
    char line[L_SIZE];
    std::ostringstream tmp;

    cmd+= fileName;
    FILE* pProc = popen(cmd.c_str(), "r");

    if (!pProc) return false;

    while (fgets(line, L_SIZE, pProc))
    {
        tmp << line;
    }
    stream.str(tmp.str());

    return pclose(pProc)==0;
}