#ifndef KLIENT_HH
#define KLIENT_HH

#include <vector>
#include <thread>
#include <mutex>
#include "AccessControl.hh"
#include "Port.hh"

// Liczba stanów (używana do animacji)
#define STATES_NUMBER 8

// Tablice poleceń (definicje w src/klient.cpp)
extern const char* Cmds4Obj1[STATES_NUMBER];
extern const char* Cmds4Obj2[STATES_NUMBER];
extern const char* Cmds4Obj3[STATES_NUMBER];

// Prosty obiekt geometryczny (namiastka MobileObj)
class GeomObject {
  int _StateIdx = 0;
  const char** _Cmd4StatDesc = nullptr;
public:
  void SetCmds(const char *CmdsTab[STATES_NUMBER]) { _Cmd4StatDesc = CmdsTab; }
  const char* GetStateDesc() const { return _Cmd4StatDesc[_StateIdx]; }
  bool IncStateIndex() { if (_StateIdx >= STATES_NUMBER-1) return false; ++_StateIdx; return true; }
};

// Prosta scena zawierająca trzy obiekty
class Scene: public AccessControl {
public:
  Scene();
  std::vector<GeomObject> _Container4Objects;
};

// Nadajnik wysyłający opisy sceny do serwera
class Sender {
  volatile bool _ContinueLooping = true;
  int _Socket = 0;
  Scene* _pScn = nullptr;
public:
  Sender(int Socket, Scene *pScn);
  bool ShouldCountinueLooping() const { return _ContinueLooping; }
  void CancelCountinueLooping() { _ContinueLooping = false; }
  void Watching_and_Sending();
};

// Funkcje udostępniane z klienta
int Send(int Sk2Server, const char *sMesg);
void Fun_CommunicationThread(Sender *pSender);
bool OpenConnection(int &rSocket);
bool ChangeState(Scene &Scn);

#endif // KLIENT_HH
