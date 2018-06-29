#include "Cpg.h"

#include <iostream>
using namespace std;

Cpg::Cpg (std::string base, std::string user, std::string password,
  std::string host, int port):
  dbName(base),
  login(user),
  password(password),
  host(host),
  port(port),
  status(false)
{
}

void Cpg::open () {
  string connStr = "dbname = '" + dbName +
                   "' user = '" + login +
                   "' password = '" + password +
                   "' hostaddr = '" + host +
                   "' port = " + to_string(port);
  try {
    conn = new pqxx::connection(connStr);
    if (conn->is_open()) {
      //  cout << "Opened database successfully: " << conn->dbname() << endl;
       status = true;
    } else {
      //  cout << "Can't open database" << endl;
    }
  } catch (const std::exception &e) {
    status = false;
    //  cerr << e.what() << std::endl;
  }
}

void Cpg::exec (std::string sql) {
  W = new pqxx::work(*conn);
  try {
    W->exec( sql );
    W->commit();
  } catch (const std::exception &e) {
    // cerr << e.what() << std::endl;
  }
}

void Cpg::createTable(std::string table) {
  exec ("CREATE TABLE IF NOT EXISTS " + table);
}

// void Cpg::createTable() {
//   string sql;
//   sql = "CREATE TABLE IF NOT EXISTS posts ("
//         "id SERIAL PRIMARY KEY ,"
//         "title VARCHAR(50),"
//         "section VARCHAR(50),"
//         "post JSONB"
//         ");";
//   exec (sql);
//   sql = "CREATE TABLE IF NOT EXISTS projects ("
//         "id SERIAL PRIMARY KEY ,"
//         "section VARCHAR(50),"
//         "CONSTRAINT production UNIQUE(section)"
//         ");";
//   exec (sql);
//   sql = "CREATE TABLE IF NOT EXISTS infoPosts ("
//         "id SERIAL PRIMARY KEY ,"
//         "creator VARCHAR(50),"
//         "post JSONB"
//         ");";
//   exec (sql);
//   sql = "CREATE TABLE IF NOT EXISTS misc ("
//         "id SERIAL PRIMARY KEY ,"
//         "misc VARCHAR(50),"
//         "data JSONB,"
//         "CONSTRAINT production2 UNIQUE(misc)"
//         ");";
//   exec (sql);
//   sql = "CREATE TABLE IF NOT EXISTS achievements ("
//         "id SERIAL PRIMARY KEY ,"
//         "login VARCHAR(50),"
//         "data JSONB,"
//         "CONSTRAINT production3 UNIQUE(login)"
//         ");";
//   exec (sql);
//   sql = "CREATE TABLE IF NOT EXISTS teachers ("
//         "id SERIAL PRIMARY KEY ,"
//         "login VARCHAR(50),"
//         "data JSONB,"
//         "CONSTRAINT production4 UNIQUE(login)"
//         ");";
//   exec (sql);
// }

void Cpg::insertPost(std::string section, std::string title, std::string post, std::string status ) {
// void Cpg::insertPost(std::string section,                      std::string title,                                std::string post) {
  string sql = "INSERT INTO posts(section,title,post)"
        "VALUES ('" +section + "','" + title + "',"
        "'" + post + "'::jsonb || jsonb_build_object('id',currval('posts_id_seq')) "
        " || jsonb_build_object('title','" + title + "') "
        " || jsonb_build_object('status','" + status + "') "
        " );";
  exec (sql);
}

void Cpg::insertTeacher(std::string login, std::string data) {
  string sql = "INSERT INTO teachers(login,data)"
  "VALUES ('" + login + "','" + data + "'::jsonb )"
  "ON CONFLICT (login)"
  "DO UPDATE SET data='" + data + "'::jsonb";
  std::cout << "sql: " << sql << '\n';
  exec (sql);
}

void Cpg::insertTeachers(std::string data) {
  Json::Value list = strToJSON(data);
  for( Json::ValueIterator it = list.begin() ; it != list.end() ; it++ ) {
    std::string name =  (*it)["people"].asString();
    std::string forJsonb = jsonToStr(*it);
    insertTeacher(name,forJsonb);
  }
}

Json::Value Cpg::getTeachers() {
  Json::Value arr = Json::Value(Json::arrayValue);
  std::vector<string> infoposts;
  string post;
  string sql = "select * from teachers;";
  pqxx::nontransaction N(*conn);
  pqxx::result R( N.exec( sql ));
  for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
    post = c[2].as<string>();
    arr.append(strToJSON(post));
  }

  return arr;
}

void    Cpg::insertAchivements(std::string login, std::string data) {
  string sql = "INSERT INTO achievements(login,data)"
        "VALUES ('" +login + "','" + data + "'::jsonb )"
        "ON CONFLICT (login)"
        "DO UPDATE SET data='" + data + "'::jsonb";
  exec (sql);
}

Json::Value Cpg::getAchivements() {
  Json::Value arr = Json::Value(Json::arrayValue);
  // arr.append(i);
  std::vector<string> infoposts;
  string post;
  // string sql = "SELECT * FROM achivements order by id desc;";
  string sql = "select * from achievements;";
  pqxx::nontransaction N(*conn);
  pqxx::result R( N.exec( sql ));
  for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
    post = c[2].as<string>();
    // infoposts.push_back (post);
    arr.append(strToJSON(post));
  }

  return arr;
}

void    Cpg::insertMisc(std::string misc, std::string data ) {
  string sql = "INSERT INTO misc(misc,data)"
        "VALUES ('" +misc + "','" + data + "'::jsonb )"
        "ON CONFLICT (misc)"
        "DO UPDATE SET data='" + data + "'::jsonb";
  exec (sql);
}

std::string Cpg::getMisc(std::string misc) {
  string sql = "select data from misc where misc='" + misc + "';";
  pqxx::nontransaction N(*conn);
  pqxx::result R( N.exec( sql ));
  pqxx::result::const_iterator c = R.begin();
  // cout << ">> " << c[0].size() << endl;
  if (c[0].size() > 0)
    return c[0].as<string>();
  else
    return "[]";
}

void Cpg::insertInfoPost (std::string creator, std::string post) {
  string sql = "INSERT INTO infoPosts(creator,post)"
        "VALUES ('" + creator + "',"
        "'" + post + "'::jsonb || jsonb_build_object('id',currval('infoPosts_id_seq')) "
        " );";

  exec (sql);
}
void Cpg::appendInfoPost(std::string strId, std::string post) {
    string sql = "UPDATE infoposts "
  "SET post = post || concat('{\"posts\":',(select (select (select post from infoposts where id="+strId+")->'posts')||'"+post+"'),'}')::jsonb "
  "WHERE id = "+strId+";";
  exec (sql);
}
Json::Value Cpg::getInfoPosts()
{
  Json::Value arr = Json::Value(Json::arrayValue);
  // arr.append(i);
  std::vector<string> infoposts;
  string post;
  string sql = "SELECT * FROM infoposts order by id desc;";
  pqxx::nontransaction N(*conn);
  pqxx::result R( N.exec( sql ));
  for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
    post = c[2].as<string>();
    // infoposts.push_back (post);
    arr.append(strToJSON(post));
  }

  return arr;
}


std::vector<string> Cpg::getProjects() {
  std::vector<string> projects;
  string project;
  string sql = "SELECT * FROM projects;";
  pqxx::nontransaction N(*conn);
  pqxx::result R( N.exec( sql ));
  for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
    project = c[1].as<string>();
    // cout << "ID = " << project << endl;
    projects.push_back (project);
  }
  // for (auto i: projects)
    // std::cout << "\t> " << i << '\n';

  return projects;
}


void Cpg::insertProject(std::string section) {
  string sql = "INSERT INTO projects(section)"
        "VALUES ('" +section + "');";
  exec (sql);
}

void Cpg::updatePost(int id, std::string post) {
  string sql = "UPDATE posts SET post = '" + post + "'::jsonb || jsonb_build_object('id',"+to_string(id)+") WHERE id = "+to_string(id)+";";
  // string sql = "INSERT INTO posts(section,title,post)"
  //       "VALUES ('" +section + "','" + title + "',"
  //       "'" + post + "'::jsonb|| jsonb_build_object('id',currval('posts_id_seq')));";
  exec (sql);
}

// DELETE FROM POSTS WHERE id=6;
void Cpg::deletePost(int id) {
  string sql = "DELETE FROM POSTS WHERE id="+to_string(id);
  exec (sql);
}

std::string Cpg::getPostsFromSection(std::string section) {
  // SELECT json_agg(post::jsonb || jsonb_build_object('id',id)) result FROM posts WHERE section='main';
  string sql = "SELECT json_agg(post) result FROM posts WHERE section='" + section + "';";
  // string sql = "SELECT json_agg(post) result FROM posts WHERE section='" + section + "';";
  // cout << "getSQL: " << sql << endl;
  pqxx::nontransaction N(*conn);
  pqxx::result R( N.exec( sql ));
  pqxx::result::const_iterator c = R.begin();
  // cout << ">> " << c[0].size() << endl;
  if (c[0].size() > 0)
    return c[0].as<string>();
  else
    return "[]";
}

Cpg::~Cpg () {
  conn->disconnect ();
  delete conn;
  // cout << "delete conn" << endl;
}
