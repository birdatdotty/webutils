#include "auth.h"

// #include <iostream>
#include <string>

#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

#include <fstream>                        // read or write file

Auth::Auth (std::string file) {
  loadFromJSONFile (file);
}

std::string Auth::addUser (std::string user) {
  boost::uuids::uuid uuid = boost::uuids::random_generator()();
  std::string key = boost::uuids::to_string(uuid);
  m_users[key] = user;
  return key;
}

std::string Auth::getUser (std::string key) {
  return m_users[key];
}

std::vector<std::string> Auth::getGroups (std::string login) {
  std::vector<std::string> tmp;
  for (auto i: loginAndGroup[login]) {
    tmp.push_back(i);
  }
  return tmp;
}

bool Auth::testGroup (std::string login, std::string group) {
  for (auto i: loginAndGroup[login]) {
    if (i == group)
    {
      return true;
    }
  }
  return false;

}


void Auth::loadFromJSONFile (std::string file) {
  jsonFile = file;
  std::ifstream ifs(file);
  Json::Value obj;
  std::string login, password;
  reader.parse(ifs, obj);
   for (int i = 0; i < obj.size(); i++){
     login = obj[i]["login"].asString();
     password = obj[i]["password"].asString();
    //  std::cout << "login/password: " << login << "/" << password << std::endl;
     loginAndPassword[obj[i]["login"].asString()] = obj[i]["password"].asString();
     if (obj[i].isMember("groups")) {
       Json::Value groups = obj[i]["groups"];
       for (Json::ValueIterator it=groups.begin(); it != groups.end(); it++)
       {
         std::string name = (*it).asString();
        //  std::cout << "obj["<< login <<"][\"groups\"]: " << name << std::endl;
         loginAndGroup[login].insert(name);
       }
     }
   }

  ifs.close();
}

bool Auth::validPassord (std::string login, std::string password) {
  if (login.size() == 0 || password.size() == 0 )
    return false;

  return password == loginAndPassword[login];
}

void Auth::addGroup (std::string login, std::string group) {
  // for (auto oo: loginAndGroup)
    // std::cout << "group: " << "oo" << std::endl;

  loginAndGroup[login].insert(group);
  // for (auto oo: loginAndGroup)
  //   std::cout << "group: " << "oo" << std::endl;
}

void Auth::createAccount (std::string login, std::string password) {
  loginAndPassword[login] = password;
  loginAndGroup[login].insert (login);
}

void Auth::removeAccount (std::string login) {
  for(auto it = loginAndPassword.begin(); it != loginAndPassword.end(); )
    if(it->first  == login)
        it = loginAndPassword.erase(it);
    else
        ++it;
}

void Auth::updateFileFromJSON () {
  updateFileFromJSON(jsonFile);
}


void Auth::updateFileFromJSON (std::string file) {
  Json::Value root = Json::Value(Json::arrayValue);
  for(auto it = loginAndPassword.cbegin(); it != loginAndPassword.cend(); ++it)
  {
    Json::Value line;
    std::string login = it->first;
    line["login"] = login;
    line["password"] = it->second;
    Json::Value groups = Json::Value(Json::arrayValue);
    for (auto i: loginAndGroup[login])
      groups.append(i);
    line["groups"] = groups;
    root.append(line);
  }
  Json::StyledWriter writer;
  std::string outputConfig = writer.write( root );
  // std::cout << "outputConfig: " << outputConfig << std::endl;
  std::ofstream ofs(file);
  if (ofs.is_open()) {
    ofs << outputConfig;
  }
  ofs.close();
}
