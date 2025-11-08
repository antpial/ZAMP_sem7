#ifndef PREPROC_HH
#define PREPROC_HH

#include <sstream>
#include <iostream>

# define L_SIZE 1024

bool preProc(const char* fileName, std::istringstream &stream);

int showPreProcesed(std::string com_file_name);

#endif // PREPROC_HH