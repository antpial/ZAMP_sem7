#include "PluginLoader.hh"
#include <dlfcn.h>


std::map<std::string, LibInterface*> loadPlugins(Configuration Config){

    void* pLibHnd; // uchwyt do bibliotek dynamicznych
    std::vector <void*>  libHandles; // wektor uchwytow do wczytanych bibliotek
    void *pFun; // uchwyt do funkcji w bibliotekach dynamicznych
    AbstractInterp4Command *(*pCreateCmd)(void); // wskaźnik do funkcji tworzącej obiekt polecenia
    AbstractInterp4Command *pCmd;
    std::map<std::string, LibInterface* > LibInterfacesMap;
    for(std::string plugin : Config.plugins) {

      // DEBUG: sprawdzam czy wtyczki wczytane z konfiguracji sa poprawne
      // std::cout << "Plugin: " << plugin << "\n";

      // Tworze uchwyty dla kolejnych pluginow
      pLibHnd = dlopen(plugin.c_str(),RTLD_LAZY);
      if (!pLibHnd) {
        std::cerr << "!!! Brak biblioteki: " << plugin << " !" << std::endl;
        exit(1);
      }
      libHandles.push_back(pLibHnd); // zapisuje uchwyt do wektora na pozniej
      
      // Pobieram adres funkcji CreateCmd z biblioteki
      pFun = dlsym(pLibHnd,"CreateCmd"); // dlsym zwraca voida* dlatego potem trzeba castowac
      if (!pFun) {
        std::cerr << "!!! Nie znaleziono funkcji CreateCmd" << std::endl;
        exit(1);
      }
      pCreateCmd = reinterpret_cast<AbstractInterp4Command* (*)(void)>(pFun); // rzutowanie na odpowiedni typ

      // Tworze obiekt do spisania nazwy polecenia
      pCmd = pCreateCmd(); // tworze obiekt polecenia np. MOVE jak w pluginie Interp4Move,cpp
      
      // Tworze strukturę LibInterface i dodaje do mapy zeby potem wywolywac
      LibInterface* libIntf = new LibInterface();
      libIntf->LibHandler = pLibHnd;
      libIntf->CmdName = pCmd->GetCmdName();
      libIntf->_pCreateCmd = pCreateCmd;
      LibInterfacesMap[libIntf->CmdName] = libIntf;

      delete pCmd;
    }

    return LibInterfacesMap;
}


int deletePlugins(std::map<std::string, LibInterface*> LibInterfacesMap){

    for (const auto& [key, value] : LibInterfacesMap) {
        std::cout << "  Usuwam " << key << "\n";
        dlclose(value->LibHandler);
        delete value;
    }

    return 0;
}

int testPlugins(std::map<std::string, LibInterface*> LibInterfacesMap){

    std::cout << "\nTestowanie wtyczek...\n";

    for (const auto& [key, value] : LibInterfacesMap) {
        std::cout << "Wtyczka dla polecenia: " << key << "\n";
        // Tworze obiekt polecenia
        AbstractInterp4Command* cmd = value->_pCreateCmd();
        // Testuje czy polecenie działa poprawnie
        std::cout << std::endl;
        std::cout << cmd->GetCmdName() << std::endl;
        std::cout << std::endl;
        cmd->PrintSyntax();
        std::cout << std::endl;
        cmd->PrintCmd();
        std::cout << std::endl;

        delete cmd;
    }

    return 0;
}

