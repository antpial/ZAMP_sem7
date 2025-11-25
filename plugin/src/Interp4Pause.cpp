#include <iostream>
#include "Interp4Pause.hh"
#include <unistd.h>



using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Pause"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Pause::CreateCmd();
}


/*!
 *
 */
Interp4Pause::Interp4Pause(): _czas_pauzy_ms(0)
{}


/*!
 *
 */
void Interp4Pause::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << " " << _czas_pauzy_ms << endl;
}


/*!
 *
 */
const char* Interp4Pause::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Pause::ExecCmd( AbstractScene      &rScn, 
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

    rComChann.LockAccess();
    usleep(this->_czas_pauzy_ms * 1000);
    rComChann.UnlockAccess();





  return true;
}


/*!
 *
 */
bool Interp4Pause::ReadParams(std::istream& Strm_CmdsList)
{
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */

  Strm_CmdsList >> _nazwa_obiektu >> _czas_pauzy_ms;

  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Pause::CreateCmd()
{
  return new Interp4Pause();
}


/*!
 *
 */
void Interp4Pause::PrintSyntax() const
{
  cout << "   Pause Nazwa_obiektu Czas_pauzy_ms" << endl;
}
