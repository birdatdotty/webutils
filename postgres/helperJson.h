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
// Json::Value combineJson (Json::Value tmp1, Json::Value tmp2);
std::string jsonToStr (Json::Value);
std::string getLoadTask (Json::Value tasks, std::string select);
std::string getLoadPrj (std::vector<std::string> tasks);
std::string getCreater (Json::Value tmp);
std::string getId (Json::Value tmp);

#endif
