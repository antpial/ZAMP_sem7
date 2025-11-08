#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include <string>
#include <vector> 


class Configuration {
  
  public:
  
  std::vector <std::string>  pluginsVec = {
      "libInterp4Move.so",
      "libInterp4Rotate.so",
      "libInterp4Set.so",
      "libInterp4Pause.so"
    };



};


#endif
