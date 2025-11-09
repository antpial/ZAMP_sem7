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
//   if (!xmlInterp.ReadFile("config/config.xml", Config))
//      return 1;

  ///////////////////////////////////////
  // Laczenie sie z serwerem
  ///////////////////////////////////////

  cout << "Port: " << PORT << endl;
  Scene               Scn;
  int                 Socket4Sending; 

  if (!OpenConnection(Socket4Sending)) return 1;

  Sender   ClientSender(Socket4Sending,&Scn);
  thread   Thread4Sending(Fun_CommunicationThread,&ClientSender);
  const char *sConfigCmds =
     "Clear\n"
     "AddObj Name=Podstawa1 RGB=(20,200,200) Scale=(4,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,20) Trans_m=(-1,3,0)\n"
     "AddObj Name=Podstawa1.Ramie1 RGB=(200,0,0) Scale=(3,3,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(4,0,0)\n"
     "AddObj Name=Podstawa1.Ramie1.Ramie2 RGB=(100,200,0) Scale=(2,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(3,0,0)\n"       
     "AddObj Name=Podstawa2 RGB=(20,200,200) Scale=(4,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(-1,-3,0)\n"
     "AddObj Name=Podstawa2.Ramie1 RGB=(200,0,0) Scale=(3,3,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(4,0,0)\n"
     "AddObj Name=Podstawa2.Ramie1.Ramie2 RGB=(100,200,0) Scale=(2,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(3,0,0)\n";
  cout << "Konfiguracja:" << endl;
  cout << sConfigCmds << endl;

  Send(Socket4Sending,sConfigCmds);
  cout << "Akcja:" << endl;    
  for (GeomObject &rObj : Scn._Container4Objects) {
    usleep(20000);
    ChangeState(Scn);
    Scn.MarkChange();
    usleep(100000);
  }
  usleep(100000);

    //-------------------------------------
  // Należy pamiętać o zamknięciu połączenia.
  // Bez tego serwer nie będzie reagował na
  // nowe połączenia.
  //
  cout << "Close\n" << endl; // To tylko, aby pokazac wysylana instrukcje
  Send(Socket4Sending,"Close\n");
  ClientSender.CancelCountinueLooping();
  Thread4Sending.join();
  close(Socket4Sending);

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