#include "Scene.hh"
#include "klient.hh"


Scene::Scene(): _Container4Objects(3)
{
    _Container4Objects[0].SetCmds(Cmds4Obj1);
    _Container4Objects[1].SetCmds(Cmds4Obj2);
    _Container4Objects[2].SetCmds(Cmds4Obj3);
}

Scene::~Scene()
{
}

AbstractMobileObj*  Scene::FindMobileObj(const char *sName) {
    for (auto& obj : MobileObjects_) {
        if (obj->GetName() == sName) {
            return obj;
        }
    }
    return nullptr;
}

void Scene::AddMobileObj(AbstractMobileObj *pMobObj) {
    MobileObjects_.push_back(pMobObj);
}