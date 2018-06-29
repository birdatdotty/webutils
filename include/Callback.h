#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include <string>
#include <functional>
#include "Reducer.h"
//#include "account.h"

class Callback {
  public:
    virtual void getMessage ( std::string,
                              std::function<void(std::string)> retClient,
                              Reducer* reducer)=0;
    virtual void setMessageAll (std::function<void(std::string)> retSendAll)=0;
    virtual std::string xhr (std::string url, std::string body)=0;
};

#endif
