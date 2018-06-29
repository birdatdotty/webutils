#ifndef _PROXY_H_
#define _PROXY_H_

#include <string>
#include <functional>
#include <Callback.h>
#include <account.h>
#include <auth.h>
#include <reducer.h>

class proxy: public Callback {
  private:
    std::function<void(std::string)> retSendAll;
    Auth *auth;
    Reducer *reducers;
  public:
    void getMessage ( std::string ctx,
                      std::function<void(std::string)> retClient,
                      Reducer* account);
    void setMessageAll (std::function<void(std::string)> retSendAll);
    std::string xhr (std::string url, std::string body);
    proxy ();
};

#endif
