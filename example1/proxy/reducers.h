#ifndef _REDUCERS_H_
#define _REDUCERS_H_

#include <auth.h>
#include <functional>
#include <account.h>

struct Reducers {
  Auth* auth;
  Account *acc;
  std::function<void(std::string)> retClient;
  void setAuth (Auth& tmp) {auth = &tmp;}
  void setRetClient (std::function<void(std::string)> f) {retClient = f;}
  void setAccount (Account *tmp) {acc = tmp;}
};

#endif
