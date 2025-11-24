#include "MobileObj.hh"
#include "Vector3D.hh"
#include <string>

MobileObj::MobileObj(/* args */)
{
    Ang_Roll_deg_ = 0.0;
    Ang_Pitch_deg_ = 0.0;
    Ang_Yaw_deg_ = 0.0;
    Position_m_[0] = 0.0;
    Position_m_[1] = 0.0;
    Position_m_[2] = 0.0;

    Name_ = "Unnamed";
}

MobileObj::~MobileObj()
{
}


double MobileObj::GetAng_Roll_deg() const {
    return Ang_Roll_deg_;
}

double MobileObj::GetAng_Pitch_deg() const {
    return Ang_Pitch_deg_;
}

double MobileObj::GetAng_Yaw_deg() const {
    return Ang_Yaw_deg_;
}

const Vector3D & MobileObj::GetPositoin_m() const {
    return Position_m_;
}

const std::string & MobileObj::GetName() const {
    return Name_;
}
