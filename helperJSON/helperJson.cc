#include "helperJson.h"
#include <json/json.h>

Json::Value vectorToJSON (std::vector<std::string> input) {
  Json::Value obj = Json::Value(Json::arrayValue);
  for (auto i: input) {
    obj.append(i);
  }
return obj;
}

std::string vectorToJSONStr (std::vector<std::string> input, std::string action, std::string key) {
  Json::FastWriter writer;
  writer.omitEndingLineFeed();
  // writer.enableYAMLCompatibility();
  Json::Value obj = Json::Value(Json::arrayValue);
  for (auto i: input) {
    obj.append(i);
  }

  if (action.size() == 0)
    return writer.write(obj);

  Json::Value full;
  full["action"] = action;
  if (key.size() == 0)
    full["data"] = obj;
  else
    full["data"][key] = obj;
  return writer.write(full);

}

Json::Value strToJSON (std::string tmp) {
  Json::Value obj;
  Json::Reader reader;
  reader.parse(tmp, obj);
  return obj;
}

Json::Value setSelect (Json::Value tmp, std::string select) {
  Json::Value obj;
  obj["select"] = select;
  obj["tasks"] = tmp;
  return obj;
}

std::string getLoadTask (Json::Value tasks, std::string select){
  Json::Value load, data;
  Json::FastWriter writer;
  data["tasks"] = tasks;
  data["select"] = select;
  load["action"] = "LOAD_TASKS";
  load["data"] = data;
  return writer.write(load);
}

std::string getLoadPrj (std::vector<std::string> prjs) {
  Json::Value load, data;
  // Json::FastWriter writer;
  data = vectorToJSON(prjs);
  load["action"] = "LOAD_PRJ";
  load["data"] = data;

  return jsonToStr(load);
    // addAction(vectorToJSON(prjs),"LOAD_PRJ"));
}

Json::Value addAction (Json::Value tmp, std::string action) {
  Json::Value obj;
  obj["action"] = action;
  obj["data"] = tmp;
  return obj;
}

Json::Value addKey (Json::Value tmp, std::string key){
  Json::Value obj;
  obj[key] = tmp;
  return obj;
}

std::string jsonToStr(Json::Value tmp) {
  Json::FastWriter writer;
  writer.omitEndingLineFeed();
  return writer.write(tmp);
}

std::string getCreater (Json::Value tmp) {
  std::string ret = tmp["creater"]["name"].asString() + " - " + tmp["creater"]["time"].asString();
  return ret;
}

std::string getId (Json::Value tmp) {
  return tmp["id"].asString();
}
