#ifndef PLUGINLOADER_HH
#define PLUGINLOADER_HH

#include <map>
#include <string>
#include "LibInterface.hh"
#include "Configuration.hh"


std::map<std::string, LibInterface*> loadPlugins(Configuration Config);


int deletePlugins(std::map<std::string, LibInterface*> LibInterfacesMap);


int testPlugins(std::map<std::string, LibInterface*> LibInterfacesMap);


#endif