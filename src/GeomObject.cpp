#include "GeomObject.hh"


///////////////////////////////////////////
// Klasa obiektu geometrycznego
///////////////////////////////////////////

  /*!
   * \brief Ustawia zestaw poleceń odpowiadających kolejnym stanom
   *        obiektu.
   */
  void GeomObject::SetCmds(const char *CmdsTab[STATES_NUMBER]) { _Cmd4StatDesc = CmdsTab; }


  /*!
   * \brief Udostępnia kolejny zestaw poleceń umożliwiających
   *        zespołu obiektu.
   *
   * Udostępnia kolejny zestaw poleceń umożliwiających
   * zespołu obiektu. Ta metoda "udaje" metodę, która w oryginalnym
   * rozwiązaniu powinna wygenerować odpowiednie polecenie na podstawie
   * przechowywanej informacji o położeniu i orientacji obiektu.
   */
  const char* GeomObject::GetStateDesc() const
  {
    return _Cmd4StatDesc[_StateIdx];
  }
    /*!
     * \brief Zwiększa indeks stanu obiektu.
     *
     * Zwiększa indeks stanu obiektu.
     * \retval true - jeśli indeks został zwiększony,
     * \retval false - jeśli indeks nie może zostać zwiększony
     *                 (osiągnięto maksymalny stan obiektu).
     */

bool GeomObject::IncStateIndex() {
    if (_StateIdx >= STATES_NUMBER-1) return false;
    ++_StateIdx;
    return true;
  }
