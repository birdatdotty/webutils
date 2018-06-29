#include "proxy.h"
#include <string>

std::string proxy::xhr (std::string url, std::string body) {
  if (url == "/index.html")
    return
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
      "<meta charset='UTF-8'/>"
      "<title>simple test</title>"
    "</head>"
    "<body>"
    "<h2>пробное содежимое:)</h2>"
    "</body>"
    "</html>";
  return "It's test from proxy!\n url:" + url+"\n body:"+body+ "\n-----------------\n";
}
