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
    MobileObj(
        const std::string &Name,
        double Roll_deg = 0.0,
        double Pitch_deg = 0.0,
        double Yaw_deg = 0.0,
        double X_m = 0.0,
        double Y_m = 0.0,
        double Z_m = 0.0
    );

    ~MobileObj();


    // Gettery
    double GetAng_Roll_deg() const override { return Ang_Roll_deg_; }
    double GetAng_Pitch_deg() const override { return Ang_Pitch_deg_; }
    double GetAng_Yaw_deg() const override { return Ang_Yaw_deg_; }
    const Vector3D & GetPositoin_m() const override { return Position_m_; } // literówka zachowana jeśli w abstrakcji
    const std::string & GetName() const override { return Name_; }

    // Settery
    void SetAng_Roll_deg(double Ang_Roll_deg) override { Ang_Roll_deg_ = Ang_Roll_deg; }
    void SetAng_Pitch_deg(double Ang_Pitch_deg) override { Ang_Pitch_deg_ = Ang_Pitch_deg; }
    void SetAng_Yaw_deg(double Ang_Yaw_deg) override { Ang_Yaw_deg_ = Ang_Yaw_deg; }
    void SetPosition_m(const Vector3D &rPos) override { Position_m_ = rPos; }
    void SetName(const char* sName) override { Name_ = sName; }
};











#endif  // MOBILEOBJ_HH