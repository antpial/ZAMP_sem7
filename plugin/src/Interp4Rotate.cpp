#include <iostream>
#include "Interp4Rotate.hh"
#include "Vector3D.hh"
#include <unistd.h>
#include "MobileObj.hh"


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Rotate"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Rotate::CreateCmd();
}


/*!
 *
 */
Interp4Rotate::Interp4Rotate(): _nazwa_obiektu("noname"), 
                                _nazwa_osi("OX"),
                                _szybkosc_katowa(0.0),
                                _kat_obrotu(0.0)
{}                               


/*!
 *
 */
void Interp4Rotate::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " 
       << _nazwa_obiektu << " "
       << _nazwa_osi << " "
       << _szybkosc_katowa << " "
       << _kat_obrotu << std::endl;
}


/*!
 *
 */
const char* Interp4Rotate::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Rotate::ExecCmd( AbstractScene      &rScn, 
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

    if( this->_nazwa_osi == "OX" )
    {

        rComChann.LockAccess();
        double start = wObMob->GetAng_Pitch_deg();
        double delta_deg = 0;
        double dist_step_deg = (double)_kat_obrotu/N;
        double time_step_us = (abs(((double)this->_kat_obrotu/this->_szybkosc_katowa))*1000000)/N;
        rComChann.UnlockAccess();

        for(int i = 0; i<N; ++i)
        {
            rComChann.LockAccess();
            delta_deg += dist_step_deg;
            wObMob->SetAng_Pitch_deg(delta_deg + start);

            rComChann.Send(rScn.UpdateObj(wObMob).c_str());

            rComChann.UnlockAccess();

            usleep(time_step_us);
        }

    }
    else if( this->_nazwa_osi == "OY" )
    {
        rComChann.LockAccess();
        double start = wObMob->GetAng_Roll_deg();
        double delta_deg = 0;
        double dist_step_deg = (double)_kat_obrotu/N;
        double time_step_us = ((abs((double)this->_kat_obrotu/this->_szybkosc_katowa))*1000000)/N;
        rComChann.UnlockAccess();

        for(int i = 0; i<N; ++i)
        {
            rComChann.LockAccess();
            delta_deg += dist_step_deg;
            wObMob->SetAng_Roll_deg(delta_deg + start);

            rComChann.Send(rScn.UpdateObj(wObMob).c_str());


            rComChann.UnlockAccess();

            usleep(time_step_us);
        }

    }
    else if( this->_nazwa_osi == "OZ" )
    {
        rComChann.LockAccess();
        double start = wObMob->GetAng_Yaw_deg();
        double delta_deg = 0;
        double dist_step_deg = (double)_kat_obrotu/N;
        double time_step_us = ((abs((double)this->_kat_obrotu/this->_szybkosc_katowa)*1000000))/N;
        rComChann.UnlockAccess();

        for(int i = 0; i<N; ++i)
        {
            rComChann.LockAccess();
            delta_deg += dist_step_deg;
            wObMob->SetAng_Yaw_deg(delta_deg + start);

            rComChann.Send(rScn.UpdateObj(wObMob).c_str());


            rComChann.UnlockAccess();

            usleep(time_step_us);
        }

    }

  return true;
}


/*!
 *
 */
bool Interp4Rotate::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */

  Strm_CmdsList >> _nazwa_obiektu
                >> _nazwa_osi
                >> _szybkosc_katowa
                >> _kat_obrotu;   

  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}


/*!
 *
 */
void Interp4Rotate::PrintSyntax() const
{
  cout << "Rotate nazwa_obiektu nazwa_osi szybkosc_katowa kat_obrotu" << endl;
}
