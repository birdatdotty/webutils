#ifndef _PG_H_
#define _PG_H_

#include <string>
#include <pqxx/pqxx>
#include "helperJson.h"
// #include <jsoncpp/json/json.h>

class Cpg {
  private:
    std::string dbName;
    std::string login;
    std::string password;
    std::string host;
    int port;
    bool status;

    pqxx::connection* conn;
    pqxx::work* W;

    void exec (std::string sql);

  public:
    Cpg (std::string base, std::string user, std::string password,
      std::string host, int port);
    void open();
    void createTable(std::string);
    // void createTable();
    void insertProject(std::string section);
    void insertInfo(std::string creator);
    Json::Value getInfoPosts();
    std::vector<std::string> getProjects();
    void insertMisc (std::string misc, std::string data );
    std::string getMisc (std::string misc);
    void insertTeacher(std::string login, std::string data);
    void insertTeachers(std::string data);
    Json::Value getTeachers();
    void insertPost(std::string section,
      std::string title,
      std::string post,
      std::string status = "new");
    void insertInfoPost(std::string creator, std::string post);
    void appendInfoPost(std::string strId, std::string post);
    void insertAchivements(std::string login, std::string data);
    Json::Value getAchivements();
    void updatePost(int id, std::string post);
    void deletePost(int id);
    std::string getPostsFromSection(std::string section);
    ~Cpg();
};

#endif
