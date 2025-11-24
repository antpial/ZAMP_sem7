#ifndef KLIENT_HH
#define KLIENT_HH

#include <vector>
#include <thread>
#include <mutex>
#include "AccessControl.hh"
#include "Port.hh"
#include "Configuration.hh"
// #include "Scene.hh"
#include "GeomObject.hh"

using namespace std;

class Scene; // forward declaration bo sie zapetlaja includy


/*!
 * \brief Wysyła napis do poprzez gniazdo sieciowe.
 *
 * Wysyła napis do poprzez gniazdo sieciowe.
 * \param[in] Sk2Server - deskryptor gniazda sieciowego, poprzez które 
 *                        ma zostać wysłany napis w sensie języka C.
 * \param[in] sMesg - zawiera napis, który ma zostać wysłany poprzez
 *                    gniazdo sieciowe.
 */
int Send(int Sk2Server, const char *sMesg);


extern const char *Cmds4Obj1[];
extern const char *Cmds4Obj2[];
extern const char *Cmds4Obj3[];



//////////////////////////////////////////
// Klasa obiektu geometrycznego
//////////////////////////////////////////

// /*!
//  * \brief Ta klasa trochę udaję klasę MobileObj, która jest w zadaniu.
//  *
//  */
// class GeomObject {
//   /*!
//    * \brief Identyfikuje aktualny stan obiektu.
//    *
//    * Identyfikuje aktualny stan obiektu. Indeks ten pozwala
//    * na wybór z tablicy \link GeomObject::_Cmd4StatDesc _Cmd4StatDesc\endlink
//    * odpowiedniego polecenia dla serwera, które reprezentuje aktualne
//    * położenie i orientację obiektu.
//    */
//    int _StateIdx = 0;
//   /*!
//    * \brief Zestaw sekwencji poleceń symulujących ruch obiektu.
//    *
//    * Zestaw sekwencji poleceń symulujących ruch obiektu.
//    * Każde polecenie odpowiada kolejnym stanom obiektu.
//    */
//    const char** _Cmd4StatDesc = nullptr;
  
//  public:

//   /*!
//    * \brief Ustawia zestaw poleceń odpowiadających kolejnym stanom
//    *        obiektu.
//    */
//   void SetCmds(const char *CmdsTab[STATES_NUMBER]);
//   /*!
//    * \brief Udostępnia kolejny zestaw poleceń umożliwiających
//    *        zespołu obiektu.
//    *
//    * Udostępnia kolejny zestaw poleceń umożliwiających
//    * zespołu obiektu. Ta metoda "udaje" metodę, która w oryginalnym
//    * rozwiązaniu powinna wygenerować odpowiednie polecenie na podstawie
//    * przechowywanej informacji o położeniu i orientacji obiektu.
//    */
//   const char* GetStateDesc() const;
//   /*!
//    * \brief Zwiększa indeks stanu, o ile aktualny opis nie jest pusty.
//    *
//    *  Zwiększa indeks stanu, o ile aktualny opis nie jest pusty.
//    *  Ta metoda "udaje" metodę, która w oryginalnym rozwiązaniu
//    *  jest odpowiedzialna za zmianę stanu obiektu, tzn. zmianę jego
//    *  położenia lub orientacji.
//    */
//   bool IncStateIndex();
// };



//////////////////////////////////////////
// Klasa sceny
//////////////////////////////////////////




// /*!
//  * \brief Namiastka sceny z prostą kolekcją trzech wizualizowanych obiektów.
//  */
// class Scene: public AccessControl {
//   public:
//     Scene();
  
//   /*!
//    * \brief Prosta kolekcja obiektów sceny
//    */
//    std::vector<GeomObject>   _Container4Objects;
// };



////////////////////////////////////////////
// Klasa sendera
////////////////////////////////////////////


/*!
 * \brief Modeluje nadajnik poleceń odzwierciedlających aktualny stan sceny.
 *
 * Modeluje nadajnik poleceń odzwierciedlających aktualny stan sceny.
 * W tym celu monitoruje zmiany na scenie, a gdy je wykryje przesyła
 * informacje o aktualnej pozycji wszystkich obiektów.
 */
class Sender {
  /*!
   * \brief Wartość tego pola decyduje o kontynuacji wykonywania wątku.
   * 
   * Wartość tego pola decyduje o kontynuacji wykonywania wątku.
   * W przypadku wartości \p true, pętla wątku będzie wykonywana.
   * Natomiast wartość \p false powoduje przerwanie pętli.
   */
   volatile bool    _ContinueLooping = true;
  /*!
   * \brief Deskryptor gniazda sieciowego, poprzez które wysyłane są polecenia.
   */
   int             _Socket = 0;
  /*!
   * \brief Wskaźnik na scenę, które stan jest przesyłany w postaci
   *        poleceń do serwera graficzneg.
   */
   Scene          *_pScn = nullptr;

  
 public:
  /*!
   * \brief Inicjalizuje obiekt deskryptorem gniazda i wskaźnikiem
   *        na scenę, na zmianę stanu które będzie ten obiekt reagował.
   */
   Sender(int Socket, Scene *pScn): _Socket(Socket), _pScn(pScn) {}

  /*!
   * \brief Sprawdza, czy pętla wątku może być wykonywana.
   * 
   * Sprawdza, czy pętla wątku może być wykonywana.
   * \retval true - pętla wątku może być nadal wykonywana.
   * \retval false - w przypadku przeciwnym.
   */
   bool ShouldCountinueLooping() const;
  /*!
   * \brief Powoduje przerwanie działania pętli wątku.
   *
   * Powoduje przerwanie działania pętli wątku.
   * \warning Reakcja na tę operację nie będize natychmiastowa.
   */  
   void CancelCountinueLooping();

  /*!
   * \brief Ta metoda jest de facto treścią wątku komunikacyjnego
   *
   * Przegląda scenę i tworzy odpowiednie polecenia, a następnie
   * wysyła je do serwera.
   * \param[in] Socket - deskryptor gniazda sieciowego, poprzez które
   *                     wysyłane są polecenia.
   */
   void Watching_and_Sending();
   
  
};


void Fun_CommunicationThread(Sender  *pSender);

bool OpenConnection(int &rSocket);

bool ChangeState(Scene &Scn);

int testFromEPortal();

int drawScene(Configuration &Config);



#endif // KLIENT_HH
