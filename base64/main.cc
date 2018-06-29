#include <iostream>

#include "base64.h"

int main () {
  std::cout << "/* message */: \n" << base64_decode("YWFhYQo=") << '\n';
  std::cout << "/* message */" << '\n';
  return 0;
}
