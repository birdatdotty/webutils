#include "proxy.h"

proxy::proxy () {
  auth = new Auth("/users.json");
}
