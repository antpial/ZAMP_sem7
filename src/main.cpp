#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"
#include "PreProc.hh"

using namespace std;


int main()
{
  
  //
  // Testuje pre procesor
  // 

    std::istringstream stream;
    if (preProc("src/example.txt", stream)) {
        std::cout << "Preprocessing OK\n";
        std::string line;
        while (std::getline(stream, line)) {
            std::cout << line << "\n";
        }
    } else {
        std::cout << "Preprocessing failed\n";
    }

  //
  // testuje biblioteke libInterp4Move.so
  //

  void *pLibHnd_Move = dlopen("libInterp4Move.so",RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Move)(void);
  void *pFun;

  if (!pLibHnd_Move) {
    cerr << "!!! Brak biblioteki: Interp4Move.so" << endl;
    return 1;
  }


  pFun = dlsym(pLibHnd_Move,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Move = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  AbstractInterp4Command *pCmd = pCreateCmd_Move();

  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;
  
  delete pCmd;

  dlclose(pLibHnd_Move);


  //
  // testuje biblioteke libInterp4Rotate.so
  //

  void *pLibHnd_Rotate = dlopen("libInterp4Rotate.so",RTLD_LAZY);
  AbstractInterp4Command *(*pCreateCmd_Rotate)(void);

  if (!pLibHnd_Rotate) {
    cerr << "!!! Brak biblioteki: Interp4Rotate.so" << endl;
    return 1;
  }


  pFun = dlsym(pLibHnd_Rotate,"CreateCmd");
  if (!pFun) {
    cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
    return 1;
  }
  pCreateCmd_Rotate = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun);


  pCmd = pCreateCmd_Rotate();

  cout << endl;
  cout << pCmd->GetCmdName() << endl;
  cout << endl;
  pCmd->PrintSyntax();
  cout << endl;
  pCmd->PrintCmd();
  cout << endl;
  
  delete pCmd;

  dlclose(pLibHnd_Rotate);

}
