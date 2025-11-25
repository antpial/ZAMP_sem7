#ifndef SCENE_HH
#define SCENE_HH

#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"
#include <vector>
// #include "klient.hh"
#include "AccessControl.hh"
#include "GeomObject.hh"
#include "MobileObj.hh"


class Scene : public AbstractScene, public AccessControl
{
private:
    std::vector<AbstractMobileObj*> MobileObjects_;
    Configuration& config;
public:
    Scene(Configuration& Config); 
    ~Scene();

    AbstractMobileObj*  FindMobileObj(const char *sName) override;

    void AddMobileObj(AbstractMobileObj *pMobObj) override;
    std::vector<GeomObject>   _Container4Objects;

    void loadObjectsFromConfig();
    std::string configCmds2Str();
    std::string UpdateObj(const AbstractMobileObj*) override;

};







#endif  // SCENE_HH