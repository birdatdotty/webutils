#ifndef _HELPERJSON_H_
#define _HELPERJSON_H_

#include <string>
#include <vector>
#include <json/json.h>

Json::Value vectorToJSON (std::vector<std::string>);
std::string vectorToJSONStr (std::vector<std::string>, std::string action="", std::string key="");
Json::Value strToJSON (std::string);
Json::Value addAction (Json::Value tmp, std::string action);
Json::Value setSelect (Json::Value obj, std::string select);
Json::Value addKey (Json::Value tmp, std::string key);
std::string jsonToStr (Json::Value);
std::string getLoadTask (Json::Value tasks, std::string select);
std::string getLoadPrj (std::vector<std::string> tasks);
std::string getCreater (Json::Value tmp);
std::string getId (Json::Value tmp);

inline
std::string jsonGetValue(Json::Value& obj, std::string val) {
  return obj[val].asString();
}

// 1
inline
std::vector<std::string> getArr(Json::Value& obj, std::string val) {
  Json::Value tmp = obj[val];
  std::vector<std::string> v;
  for (Json::ValueConstIterator it = tmp.begin(); it != tmp.end(); ++it)
  {
    const Json::Value& item = *it;
    v.push_back(item.asString());
    // rest as before
  }
  return v;
}

// 2
inline
std::vector<std::string> jsonToVector(Json::Value obj) {
  Json::Value tmp = obj;
  std::vector<std::string> v;
  for (Json::ValueConstIterator it = tmp.begin(); it != tmp.end(); ++it)
  {
    const Json::Value& item = *it;
    v.push_back(item.asString());
    // rest as before
  }
  return v;
}
// 3
inline
std::vector<std::string> jsonStrToVector (std::string str) {return jsonToVector(strToJSON(str));};

// 4
inline
std::string vectorToStr (std::vector<std::string> header) {
  std::string toHeader;
  for (std::string str: header)
    toHeader += str;

  return toHeader;
}

// 5
inline
std::string jsonStrArrayToStr (std::string jsonStr) {
  return vectorToStr(jsonStrToVector(jsonStr));
}


#endif
