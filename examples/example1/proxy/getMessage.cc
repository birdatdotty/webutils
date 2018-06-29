#include "proxy.h"

#include <string>
#include <functional>

#include <iostream>

void proxy::getMessage (std::string ctx,
                        std::function<void(std::string)> retClient,
                        Account* account)
{
  retClient("lllllllllllllll");
  if (account->init) {
    std::cout << "getMessage: " << account->getName() << '\n';
  };
  account->setName(ctx);
}
