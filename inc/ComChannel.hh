#ifndef COMCHANNEL_HH
#define COMCHANNEL_HH

#include "AbstractComChannel.hh"
#include "Configuration.hh"


class ComChannel : public AbstractComChannel {
 private:
  int Socket_;
  std::mutex Mutex_;
  bool OpenConnection(int &rSocket);

 public:
  ComChannel();
  ~ComChannel() override;

  void Init(int Socket) override;
  int GetSocket() const override;
  void LockAccess() override;
  void UnlockAccess() override;
  std::mutex &UseGuard() override;
  int Send(const char *sMesg) override;

};






#endif  // COMCHANNEL_HH