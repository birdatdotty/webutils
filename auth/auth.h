#ifndef _AUTH_H_
#define _AUTH_H_

#include <map>
#include <set>
#include <string>
#include <json/json.h>
#include <vector>

class Auth {
  private:
    Json::Reader reader;
    std::map <std::string, std::string> m_users;
    std::map <std::string, std::string> loginAndPassword;
    std::map <std::string, std::set<std::string> > loginAndGroup;
    std::string jsonFile;
  public:
    Auth (std::string);// {};
    std::string addUser (std::string user);
    std::string getUser (std::string key);
    void addGroup (std::string login, std::string group);
    void createAccount (std::string login, std::string password);
    void removeAccount (std::string login);
    void updateFileFromJSON (std::string file);
    void updateFileFromJSON ();
    void loadFromJSONFile (std::string file);
    bool validPassord (std::string login, std::string password);
    bool testGroup (std::string login, std::string group);
    std::vector<std::string> getGroups (std::string login);
};

#endif
