#include <iostream>
#include <string>

#include "helperJson.h"

int main () {
  std::cout << "/* message */" << '\n';

  std::string json1 = "{\"t1\":\"test first\",\"action\":\"it's test\"}";
  Json::Value qqq = strToJSON (json1);

  std::cout << "json1:" << jsonGetValue(qqq,"action") << '\n';

  return 0;
}
