#ifndef KLIENT_HH
#define KLIENT_HH

#include <vector>
#include <thread>
#include <mutex>
#include "AccessControl.hh"
#include "Port.hh"
#include "Configuration.hh"
#include "GeomObject.hh"

using namespace std;

class Scene; // forward declaration bo sie zapetlaja includy
class Sender; // forward declaration bo sie zapetlaja includy


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


void Fun_CommunicationThread(Sender  *pSender);

bool OpenConnection(int &rSocket);

bool ChangeState(Scene &Scn);

int drawScene(Configuration &Config);



#endif // KLIENT_HH
