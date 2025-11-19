#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <string>
#include <vector> 
#include "Vector3D.hh"
#include <map>

/*!
  * \brief Struktura opisująca sześcian do umieszczenia w scenie
  * zgodnie z plikiem konfiguracyjnym XML
  *
  * Struktura opisująca sześcian do umieszczenia w scenie
  */

struct Cube
{
    std::string Name;
    std::map<std::string, Vector3D>  ParamsMap;
};



class Configuration {
  
  public:

  std::vector <std::string> plugins;

  /*!
    * \brief Kolekcja opisów sześcianów do umieszczenia w scenie
  */
  std::vector <Cube*>  cubesVec;

};


#endif
