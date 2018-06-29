#include "createHTML.h"
#include <cstring>

bool createHTML (std::string &html, std::string tmp, std::string header, std::string ctx, std::string end) {
  // std::string toHeader;
  // for (std::string str: header)
  //   toHeader += str;

  int size = tmp.size() +
             header.size() +
             ctx.size() +
             end.size();

  char *HTML =(char*) malloc(size);
  sprintf(HTML, tmp.c_str(), header.c_str(), ctx.c_str(), end.c_str() );
  *(HTML+size) = '\0';

  std::string ret = std::string(HTML);
              html = std::string(HTML);
  free(HTML);

  return true;
}
