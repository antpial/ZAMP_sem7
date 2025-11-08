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

#define COM_FILE_NAME "opis_dzialan.cmd"

using namespace std;
using namespace xercesc;


 /* Czyta z pliku opis poleceń i dodaje je do listy komend,
 * które robot musi wykonać.
 * \param sFileName - (\b we.) nazwa pliku z opisem poleceń.
 * \param CmdList - (\b we.) zarządca listy poleceń dla robota.
 * \retval true - jeśli wczytanie zostało zrealizowane poprawnie,
 * \retval false - w przeciwnym przypadku.
 */
bool ReadFile(const char* sFileName, Configuration &rConfig)
{
   try {
            XMLPlatformUtils::Initialize();
   }
   catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cerr << "Error during initialization! :\n";
            cerr << "Exception message is: \n"
                 << message << "\n";
            XMLString::release(&message);
            return 1;
   }

   SAX2XMLReader* pParser = XMLReaderFactory::createXMLReader();

   pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
   pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
   pParser->setFeature(XMLUni::fgXercesDynamic, false);
   pParser->setFeature(XMLUni::fgXercesSchema, true);
   pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);

   pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

   DefaultHandler* pHandler = new XMLInterp4Config(rConfig);
   pParser->setContentHandler(pHandler);
   pParser->setErrorHandler(pHandler);

   try {
     
     if (!pParser->loadGrammar("config/config.xsd",
                              xercesc::Grammar::SchemaGrammarType,true)) {
       cerr << "!!! Plik grammar/actions.xsd, '" << endl
            << "!!! ktory zawiera opis gramatyki, nie moze zostac wczytany."
            << endl;
       return false;
     }
     pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse,true);
     pParser->parse(sFileName);
   }
   catch (const XMLException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            cerr << "Informacja o wyjatku: \n"
                 << "   " << sMessage << "\n";
            XMLString::release(&sMessage);
            return false;
   }
   catch (const SAXParseException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            char* sSystemId = xercesc::XMLString::transcode(Exception.getSystemId());

            cerr << "Blad! " << endl
                 << "    Plik:  " << sSystemId << endl
                 << "   Linia: " << Exception.getLineNumber() << endl
                 << " Kolumna: " << Exception.getColumnNumber() << endl
                 << " Informacja: " << sMessage 
                 << endl;

            XMLString::release(&sMessage);
            XMLString::release(&sSystemId);
            return false;
   }
   catch (...) {
            cout << "Zgloszony zostal nieoczekiwany wyjatek!\n" ;
            return false;
   }

   delete pParser;
   delete pHandler;
   return true;
}


std::map<std::string, LibInterface*> loadPlugins(Configuration Config){

    void* pLibHnd; // uchwyt do bibliotek dynamicznych
    std::vector <void*>  libHandles; // wektor uchwytow do wczytanych bibliotek
    void *pFun; // uchwyt do funkcji w bibliotekach dynamicznych
    AbstractInterp4Command *(*pCreateCmd)(void); // wskaźnik do funkcji tworzącej obiekt polecenia
    AbstractInterp4Command *pCmd;
    map<std::string, LibInterface* > LibInterfacesMap;
    for(std::string plugin : Config.pluginsVec) {

      // sprawdzam czy wtyczki wczytane z konfiguracji sa poprawne
      std::cout << "Plugin: " << plugin << "\n";

      // Tworze uchwyty dla kolejnych pluginow
      pLibHnd = dlopen(plugin.c_str(),RTLD_LAZY);
      if (!pLibHnd) {
        cerr << "!!! Brak biblioteki: " << plugin << " !" << endl;
        exit(1);
      }
      libHandles.push_back(pLibHnd); // zapisuje uchwyt do wektora na pozniej
      
      // Pobieram adres funkcji CreateCmd z biblioteki
      pFun = dlsym(pLibHnd,"CreateCmd"); // dlsym zwraca voida* dlatego potem trzeba castowac
      if (!pFun) {
        cerr << "!!! Nie znaleziono funkcji CreateCmd" << endl;
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



int main()
{

  Configuration Config;

  ///////////////////////////////////////
  // Zaciagam wtyczki (pluginy)
  ///////////////////////////////////////

  std::cout << "Ładowanie wtyczek...\n";
  map<std::string, LibInterface* > LibInterfacesMap = loadPlugins(Config);

  ///////////////////////////////////////
  // Testuje czy wtyczki działają poprawnie
  ///////////////////////////////////////

  std::cout << "\nTestowanie wtyczek...\n";
  testPlugins(LibInterfacesMap);

  ////////////////////////////////////////
  // Uruchamiam preproces dla pliku z poleceniami
  ////////////////////////////////////////

  std::cout << "\nPreprocesuje plik " << COM_FILE_NAME << ":\n";

  showPreProcesed(COM_FILE_NAME); // pokazuje przeprocesowany plik

  std::istringstream stream;
  if(not preProc(COM_FILE_NAME, stream)) {
      std::cerr << "Preprocessing failed\n";
      exit(0);
  }

  std::string order;
  stream >> order;
  AbstractInterp4Command * cmd = LibInterfacesMap[order]->_pCreateCmd();
  cmd->ReadParams(stream);
  cmd->PrintCmd();



////////////////////////////////////////
// Usuwam wtyczki
///////////////////////////////////////

std::cout << "\nUsuwanie wtyczek...\n";
deletePlugins(LibInterfacesMap);

// testuje zaciaganie z xml

// if (!ReadFile("config/config.xml",Config)) return 1;


}
