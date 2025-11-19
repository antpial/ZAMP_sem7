#ifndef COMCHANNEL_HH
#define COMCHANNEL_HH

#include "AbstractComChannel.hh"


class ComChannel : public AbstractComChannel {
 private:
  int Socket_;
  std::mutex Mutex_;

 public:
  ComChannel();
  ~ComChannel() override;

  void Init(int Socket) override;
  int GetSocket() const override;
  void LockAccess() override;
  void UnlockAccess() override;
  std::mutex &UseGuard() override;
};






#endif  // COMCHANNEL_HH