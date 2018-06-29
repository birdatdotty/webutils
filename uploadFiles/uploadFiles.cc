// #include "actions.h"
#include <json/json.h>
#include <iostream>

// extern bool(*saveFile) (std::string myPath,               std::string fileName,               std::string data);
#include "../saveFile/saveFile.h"

bool uploadFiles(std::string to, Json::Value data) {
  std::cout << "uploadFile" << data << '\n';

  // for data
  // for (const auto& k : first_view(data.begin(), data.end())) {
  //   std::cout << ">>" << k << ": " << data[k]  << '\n';
  // }
  // for (const auto& kv : data) {
  //   const Json::CZString& k = kv.first;
  //   Json::Value& v = kv.second;
  //   std::cout << ">>" << k << ": " << v  << '\n';
  //
  // }
  for (Json::Value::iterator it = data.begin(); it != data.end(); ++it)
  {
    std::cout << ">>" << it.key() << ": " << *it  << '\n';
    saveFile( to,
              it.key().asString(),
              (*it).asString() );
  }

  // for(Json::Value::iterator it = change.begin(); it !=change.end(); ++it)
  // {
  //   Json::Value key = it.key();
  //   Json::Value value = (*it);
  //   cout<<"Key: "<<key.toStyledString();
  //   cout<<"Value: "<<value.toStyledString();
  // }

  return true;
}
