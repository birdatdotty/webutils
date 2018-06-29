#ifndef _ACCOUT_H_
#define _ACCOUT_H_

#include <string>

class Reducer {
  std::string name;
  public:
    bool init;
    Reducer () {
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
