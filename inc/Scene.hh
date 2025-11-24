#ifndef SCENE_HH
#define SCENE_HH

#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"
#include <vector>
// #include "klient.hh"
#include "AccessControl.hh"
#include "GeomObject.hh"


class Scene : public AbstractScene, public AccessControl
{
private:
    std::vector<AbstractMobileObj*> MobileObjects_;
public:
    Scene(/* args */);
    ~Scene();

    AbstractMobileObj*  FindMobileObj(const char *sName) override;

    void AddMobileObj(AbstractMobileObj *pMobObj) override;
    std::vector<GeomObject>   _Container4Objects;

};







#endif  // SCENE_HH