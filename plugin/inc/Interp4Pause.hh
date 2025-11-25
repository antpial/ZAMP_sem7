#ifndef  COMMAND4PAUSE_HH
#define  COMMAND4PAUSE_HH

#ifndef __GNUG__
# pragma interface
# pragma implementation
#endif

#include "AbstractInterp4Command.hh"

/*!
 * \file
 * \brief Definicja klasy Interp4Move
 *
 * Plik zawiera definicję klasy Interp4Move ...
 */

/*!
 * \brief Modeluje polecenie dla robota mobilnego, które wymusza jego ruch do przodu
 *
 *  Klasa modeluj47e ...
 */
class Interp4Pause: public AbstractInterp4Command {
  /*
   *  Tu należy zdefiniować pola, które są niezbędne
   *  do przechowywania wartości parametrów danego polecenia.
   *  Ponieżej zdefiniowane jest tylko jedno pole jako przykład.
   */
  std::string _nazwa_obiektu;
  double  _czas_pauzy_ms;
 public:
  /*!
   * \brief
   */
  Interp4Pause();  
  /*!
   * \brief Wyświetla postać bieżącego polecenia (nazwę oraz wartości parametrów)
   */
  virtual void PrintCmd() const override;
  /*!
   * \brief Wyświetla składnię polecenia
   */
  virtual void PrintSyntax() const override;
  /*!
   * \brief Wyświetla wartości wczytanych parametrów
   */
  virtual void PrintParams() const override {} 

  /*!
   * \brief Wyświetla nazwę polecenia
   */
  virtual const char* GetCmdName() const override;

  inline const char* GetObjName() const override {return _nazwa_obiektu.c_str();};


  /*!
   * \brief Wykonuje polecenie oraz wizualizuje jego realizację
   *
   * Wykonuje polecenie oraz wizualizuje jego realizację.
   * \param[in,out]  rScn - scena zawierającą obiekty mobilne,
   * \param[in]      sMobObjName - wskaźnik na nazwę lokalizującą i identyfikującą obiekt,
   * \param[in,out]  rComChann - kanał komunikacyjny z serwerem graficznym.
   * \retval true - operacja powiodła się,
   * \retval false - w przypadku przeciwnym.
   */
  virtual bool ExecCmd( AbstractScene      &rScn, 
                        const char         *sMobObjName,
                        AbstractComChannel &rComChann ) override;
  /*!
   * \brief Czyta wartości parametrów danego polecenia
   */
  virtual bool ReadParams(std::istream& Strm_CmdsList) override;

  
  /*!
   * \brief
   *
   *  Ta metoda nie musi być zdefiniowna w klasie bazowej.
   */
  static AbstractInterp4Command* CreateCmd();
 };

#endif
