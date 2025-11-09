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




int main()
{

  Configuration Config;

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

// testuje zaciaganie z xml
if (!ReadFile("config/config.xml",Config)) return 1;


}
