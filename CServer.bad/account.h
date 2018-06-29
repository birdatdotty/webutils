#ifndef _ACCOUT_H_
#define _ACCOUT_H_

#include <string>

class Account {
  std::string name;
  public:
    bool init;
    Account () {
      init=false;
    }
    void setName(std::string tmp) {
      name = tmp;
      init=true;
    }
    std::string getName() {
      return name;
    }
};

#endif
