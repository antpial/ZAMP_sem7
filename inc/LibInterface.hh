#ifndef LIBINTERFACE_HH
#define LIBINTERFACE_HH

#include <string>
#include <memory>
#include "AbstractInterp4Command.hh" 

struct LibInterface {

    // narazie przepisana ze schematu

    using CreateCmdFunc = AbstractInterp4Command* (*)(void);
    void* LibHandler = nullptr;
    std::string CmdName;                 
    CreateCmdFunc _pCreateCmd = nullptr; 
    ~LibInterface() = default;
    LibInterface() = default;
};

#endif