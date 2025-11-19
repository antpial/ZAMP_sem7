#ifndef SCENE_HH
#define SCENE_HH

#include "AbstractScene.hh"


class Scene : public AbstractScene
{
private:
    /* data */
public:
    Scene(/* args */);
    ~Scene();

    AbstractMobileObj*  FindMobileObj(const char *sName) override;

    void AddMobileObj(AbstractMobileObj *pMobObj) override;

};







#endif  // SCENE_HH