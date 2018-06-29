#include "proxy.h"

#include <string>
#include <functional>

void proxy::getMessage (std::string ctx,
                        std::function<void(std::string)> retClient,
                        Reducer* account)
{
  retClient("lllllllllllllll");
}
