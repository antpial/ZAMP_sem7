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

int showPreProcesed(std::string com_file_name){

    std::cout << "\nPrzeprocesowany plik " << com_file_name << ":\n";

    std::istringstream stream;
    if (preProc(com_file_name.c_str(), stream)) {
        std::cout << "Preprocessing OK\n";
        std::string line;
        while (std::getline(stream, line)) {
            std::cout << line << "\n";
        }
    } else {
        std::cout << "Preprocessing failed\n";
        return 1;
    }

    std::cout << "\n";

    return 0;
}