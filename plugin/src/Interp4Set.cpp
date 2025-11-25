#include <iostream>
#include "Interp4Set.hh"


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Set"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Set::CreateCmd();
}


/*!
 *
 */
Interp4Set::Interp4Set(): _nazwa_obiektu("noname"),
                         _wsp_x(0.0),
                         _wsp_y(0.0),
                         _wsp_z(0.0),
                         _kat_OX(0.0),
                         _kat_OY(0.0),
                         _kat_OZ(0.0)
{}


/*!
 *
 */
void Interp4Set::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " "
       << _nazwa_obiektu << " "
       << _wsp_x << " " << _wsp_y << " " << _wsp_z << " "
       << _kat_OX << " " << _kat_OY << " " << _kat_OZ
       << endl;
}


/*!
 *
 */
const char* Interp4Set::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Set::ExecCmd( AbstractScene      &rScn, 
                           const char         *sMobObjName,
			   AbstractComChannel &rComChann
			 )
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */


    AbstractMobileObj* wObMob = rScn.FindMobileObj(this->_nazwa_obiektu.c_str());

    if( wObMob == nullptr )
    {
        std::cerr<<"Nie mogę znaleźć obiektu: "<<this->_nazwa_obiektu.c_str()<<std::endl;
        return false;
    }

    Vector3D tmpVec;
    tmpVec[0] = _wsp_x;
    tmpVec[1] = _wsp_y;
    tmpVec[2] = _wsp_z;

    rComChann.LockAccess();

    wObMob->SetPosition_m(tmpVec);
    wObMob->SetAng_Yaw_deg(_kat_OZ);
    wObMob->SetAng_Roll_deg(_kat_OX);
    wObMob->SetAng_Pitch_deg(_kat_OY);

    rComChann.Send(rScn.UpdateObj(wObMob).c_str());

    rComChann.UnlockAccess();



  return true;
}


/*!
 *
 */
bool Interp4Set::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */

   Strm_CmdsList >> _nazwa_obiektu
                  >> _wsp_x >> _wsp_y >> _wsp_z
                  >> _kat_OX >> _kat_OY >> _kat_OZ;

  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}


/*!
 *
 */
void Interp4Set::PrintSyntax() const
{
  cout << "   Set nazwa_obiektu wsp_x wsp_y wsp_z kat_OX kat_OY kat_OZ" << endl;
}
