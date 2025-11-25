#include "Scene.hh"
#include "klient.hh"
#include "MobileObj.hh"
#include <cstring>
#include <sstream>
#include <iomanip>  // dla setprecision

using namespace std;


Scene::Scene(Configuration& Config)
    : config(Config), _Container4Objects(3) 
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

void Scene::loadObjectsFromConfig(){
    for(auto cubePtr : config.cubesVec){
        MobileObj* mobObj = new MobileObj(
            cubePtr->Name,
            cubePtr->ParamsMap.at("RotXYZ_deg")[0],
            cubePtr->ParamsMap.at("RotXYZ_deg")[1],
            cubePtr->ParamsMap.at("RotXYZ_deg")[2],
            cubePtr->ParamsMap.at("Shift")[0],
            cubePtr->ParamsMap.at("Shift")[1],
            cubePtr->ParamsMap.at("Shift")[2]
        );
        AddMobileObj(mobObj);
    }
    std::cout << "Loaded " << MobileObjects_.size() << " mobile objects from configuration." << std::endl;
}



std::string Scene::configCmds2Str(){
    std::string str = "Clear";
    for(auto cubePtr : config.cubesVec){
        str += "\nAddObj Name=" + cubePtr->Name + " ";
        for(const auto& param : cubePtr->ParamsMap){
        if(param.first == "RGB"){
            str += param.first + "=(" + 
                to_string(int(param.second[0])) + "," + 
                to_string(int(param.second[1])) + "," + 
                to_string(int(param.second[2])) + ") ";
        }else{
        str += param.first + "=(" + 
                to_string(param.second[0]) + "," + 
                to_string(param.second[1]) + "," + 
                to_string(param.second[2]) + ") ";
        }
        }
        
    }
    str += "\n";

    const char *sConfigCmds = new char[str.size() + 1];
    strcpy(const_cast<char*>(sConfigCmds), str.c_str());
    cout << "Konfiguracja:" << endl;
    cout << sConfigCmds << endl;

    return sConfigCmds;
}



std::string Scene::UpdateObj(const AbstractMobileObj* Obje) {

    std::ostringstream oss;

    // Zaczynamy od podstawowego polecenia
    // oss << "Clear\n";
    // "UpdateObj Name=Podstawa2.Ramie1 RotXYZ_deg=(0,-48,0)\n",

     for (auto& Obj : MobileObjects_){
        oss << "UpdateObj Name=" << Obj->GetName() << " ";

        // Najpierw pozycja i rotacja z MobileObj
        const Vector3D &pos = Obj->GetPositoin_m();
        oss << "Shift=(" 
            << std::fixed << std::setprecision(2) << pos[0] << "," 
            << pos[1] << "," << pos[2] << ") ";

        oss << "RotXYZ_deg=(" 
            << Obj->GetAng_Roll_deg() << "," 
            << Obj->GetAng_Pitch_deg() << "," 
            << Obj->GetAng_Yaw_deg() << ") ";


        oss << "\n";
     }
    cout << oss.str();

    return oss.str();
}

void Scene::UpdateGlobalTransforms(const std::string& root)
{
    MobileObj* obj = Find(root);
    if (!obj) return;

    // 1. policz global transform tego obiektu
    if (obj->HasParent())
        obj->global = obj->parent->global * obj->local;  // macierze transformacji
    else
        obj->global = obj->local;

    // 2. wyślij UpdateObj
    rCom.Send( MakeUpdateString(obj) );

    // 3. przejdź po dzieciach
    for (auto* child : children[obj])
        UpdateGlobalTransforms(child->Name());
}




