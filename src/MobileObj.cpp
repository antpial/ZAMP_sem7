#include "MobileObj.hh"
#include "Vector3D.hh"
#include <string>

MobileObj::MobileObj(const std::string &Name,
                     double Roll_deg,
                     double Pitch_deg,
                     double Yaw_deg,
                     double X_m,
                     double Y_m,
                     double Z_m)
  : Ang_Roll_deg_(Roll_deg),
    Ang_Pitch_deg_(Pitch_deg),
    Ang_Yaw_deg_(Yaw_deg),
    Position_m_()   // najpierw domyślnie zerujemy wektor
{
    Position_m_[0] = X_m;
    Position_m_[1] = Y_m;
    Position_m_[2] = Z_m;
    Name_ = Name;
}

MobileObj::~MobileObj()
{
}
