#ifndef MOBILEOBJ_HH
#define MOBILEOBJ_HH

#include "AbstractMobileObj.hh"
#include "Vector3D.hh"
#include <string>

class MobileObj : public AbstractMobileObj
{
private:
    double Ang_Roll_deg_;   ///< Kąt \e roll w stopniach.
    double Ang_Pitch_deg_;  ///< Kąt \e pitch w stopniach.
    double Ang_Yaw_deg_;    ///< Kąt \e yaw w stopniach.
    Vector3D Position_m_;   ///< Współrzędne pozycji obiektu w metrach.
    std::string Name_;      ///< Nazwa obiektu.
public:
    MobileObj(/* args */);
    ~MobileObj();

    double GetAng_Roll_deg() const override;
    double GetAng_Pitch_deg() const override;
    double GetAng_Yaw_deg() const override;
    const Vector3D & GetPositoin_m() const override;
    const std::string & GetName() const override;
};











#endif  // MOBILEOBJ_HH