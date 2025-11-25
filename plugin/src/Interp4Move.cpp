#include <iostream>
#include "Interp4Move.hh"
#include "Vector3D.hh"
#include <unistd.h>
#include "MobileObj.hh"


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Move"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Move::CreateCmd();
}


/*!
 *
 */
Interp4Move::Interp4Move(): _nazwa_obiektu("noname"),
                            _szybkosc(0.0),
                            _dlugosc_drogi(0.0)
{}


/*!
 *
 */
void Interp4Move::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " 
       << _nazwa_obiektu << " "
       << _szybkosc << " "
       << _dlugosc_drogi;
}


/*!
 *
 */
const char* Interp4Move::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Move::ExecCmd( AbstractScene      &rScn, 
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */

     AbstractMobileObj* wObMob = rScn.FindMobileObj(sMobObjName);

    if( wObMob == nullptr )
    {
        std::cerr<<"Nie mogę znaleźć obiektu: "<< sMobObjName <<std::endl;
        return false;
    }


  Vector3D startPos = wObMob->GetPositoin_m();
    double startRoll = wObMob->GetAng_Roll_deg();
    double startPitch = wObMob->GetAng_Pitch_deg();
    double startYaw = wObMob->GetAng_Yaw_deg();
    double delta_x_m, delta_y_m, delta_z_m;
    delta_x_m = delta_y_m = delta_z_m = 0;
    double dist_step_m = (double)_dlugosc_drogi/N;
    double time_step_us = (((double)_dlugosc_drogi/this->_szybkosc)*1000000)/N;

    for(int i = 0; i<N; ++i)
    {
        // wObMob->LockAccess();
  
        delta_x_m += dist_step_m*cos(startPitch*M_PI/180)*cos(startYaw*M_PI/180);
        delta_y_m += dist_step_m*(cos(startRoll*M_PI/180)*sin(startYaw*M_PI/180) + cos(startYaw*M_PI/180)*sin(startPitch*M_PI/180)*sin(startRoll*M_PI/180));
        delta_z_m += dist_step_m*(sin(startRoll*M_PI/180)*sin(startYaw*M_PI/180) - cos(startRoll*M_PI/180)*cos(startYaw*M_PI/180)*sin(startPitch*M_PI/180));
        Vector3D vecTmp;
        vecTmp[0] = delta_x_m+startPos[0];
        vecTmp[1] = delta_y_m+startPos[1];
        vecTmp[2] = delta_z_m+startPos[2];
        wObMob->SetPosition_m(vecTmp);

        rComChann.Send(rScn.UpdateObj(wObMob).c_str());
        usleep(time_step_us);
    }




        // wObMob->UnLockAccess();

        
    
    

 
  return true;
}


/*!
 *
 */
bool Interp4Move::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */

  Strm_CmdsList >> _nazwa_obiektu
                >> _szybkosc
                >> _dlugosc_drogi;
                
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Move::CreateCmd()
{
  return new Interp4Move();
}


/*!
 *
 */
void Interp4Move::PrintSyntax() const
{
  cout << "   Move nazwa_obiektu szybkosc dlugosc_drogi" << endl;
}
