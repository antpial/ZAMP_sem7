#include "ComChannel.hh"
#include "klient.hh"

ComChannel::ComChannel(){
    Socket_ = -1;
    Init(Socket_);
}

void ComChannel::Init(int Socket){
    try{
        OpenConnection(Socket_);
    }catch(...){
        throw std::runtime_error("ComChannel::ComChannel: Failed to open connection and to make ComChannel");
    }
}

ComChannel::~ComChannel(){
    // close(Socket_);
}

int ComChannel::GetSocket() const{
    return Socket_;
}

void ComChannel::LockAccess(){
    Mutex_.lock();
}

void ComChannel::UnlockAccess(){
    Mutex_.unlock();
}

std::mutex & ComChannel::UseGuard(){
    return Mutex_;
}