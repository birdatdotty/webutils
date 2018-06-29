#include "proxy.h"
#include <functional>

void proxy::setMessageAll (std::function<void(std::string)> retSendAll) {
  this->retSendAll = retSendAll;
}
