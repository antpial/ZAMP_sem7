#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include "AbstractInterp4Command.hh"
#include "PreProc.hh"
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>
#include "xmlinterp.hh"
#include <iostream>
#include <list>
#include <map>
#include "LibInterface.hh"
#include "PluginLoader.hh"
#include "AccessControl.hh"
#include "Port.hh"
#include "klient.hh"
#include <thread>
#include <unistd.h>

#define COM_FILE_NAME "opis_dzialan.cmd"

using namespace std;
using namespace xercesc;



int main()
{

  Configuration Config;

  ///////////////////////////////////////
  // Wczytanie pliku konfiguracyjnego XML
  ///////////////////////////////////////
  XMLInterp4Config xmlInterp(Config);
  if (!xmlInterp.ReadFile("config/config.xml", Config))
     return 1;

  // DEBUG: Pokazuje wczytane sześciany
  std::cout << "\nWczytane sześciany z pliku konfiguracyjnego:\n";
  for(const auto& cube : Config.cubesVec){
      std::cout << "Main: Cube Name = " << cube->Name << "\n";
      for (const auto& [key, value] : cube->ParamsMap){
          std::cout << "      " << key << " => " << value << "\n";
      }
  }

  ///////////////////////////////////////
  // Laczenie sie z serwerem i rysuje scene
  ///////////////////////////////////////

  // test ktory byl w klient.cpp::main na eportalu
  // testFromEPortal();

  drawScene(Config);

  ///////////////////////////////////////
  // Zaciagam wtyczki (pluginy)
  ///////////////////////////////////////

  std::cout << "\nŁadowanie wtyczek...\n";
  map<std::string, LibInterface* > LibInterfacesMap = loadPlugins(Config);
  // DEBUG: test wtyczek
  // testPlugins(LibInterfacesMap);

  ////////////////////////////////////////
  // Uruchamiam preproces
  ////////////////////////////////////////

  std::cout << "\nPreprocesuje plik " << COM_FILE_NAME << "...\n";
  // DEBUG: Pokazuje przepreprocesowany plik
  // showPreProcesed(COM_FILE_NAME);

  std::istringstream stream;
  if(not preProc(COM_FILE_NAME, stream)) {
      std::cerr << "Preprocessing failed\n";
      exit(0);
  }

  /////////////////////////////////////////
  // Realizuje kolejne komendy
  /////////////////////////////////////////

  AbstractInterp4Command * cmd;
  std::string order;

  std:: cout << "\nRealizuje kolejne komendy: \n";

  while(stream >> order){
  cmd = LibInterfacesMap[order]->_pCreateCmd();
  cmd->ReadParams(stream);
  cmd->PrintCmd();
  }


////////////////////////////////////////
// Usuwam wtyczki
///////////////////////////////////////

std::cout << "\n\nUsuwanie wtyczek...\n";
deletePlugins(LibInterfacesMap);

return 0;
}