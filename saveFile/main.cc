#include "saveFile.h"

#include <iostream>

int main ()
{
  std::cout << "/* message */" << '\n';
  // saveFile (path, obj["data"]["name"].asString(), data);

  saveFile ("/tmp/q1/","testFile.txt","Hello, world!");

  return 0;
}
