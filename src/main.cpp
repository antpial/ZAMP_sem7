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
//   if (!xmlInterp.ReadFile("config/config.xml", Config))
//      return 1;

  ///////////////////////////////////////
  // Laczenie sie z serwerem
  ///////////////////////////////////////

  cout << "Port: " << PORT << endl;
  Scene               Scn;
  int                 Socket4Sending; 

  if (!OpenConnection(Socket4Sending)) return 1;


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