#include <iostream>
#include <vector>

#include <webutils/saveFile.h>
#include <helperJson.h>

#include "createHTML.h"

#include <fstream>

#include <helperJson.h>

#define PAGE_SUFFIX ".html"
#define MENU_FILE "/var/www/html/json/menu.json"

int menu(std::string file, std::string id, std::string title);

int www (std::string HTML_DIR, std::string bodyStr) {
  const std::string tplsStr =
        "{\"default\":\"<!DOCTYPE html><html><head><meta charset='utf-8'>"
        "<meta name='viewport' content='width=device-width, initial-scale=1,"
        "shrink-to-fit=no'><meta name='theme-color' content='#000000'>%s</head>"
        "<body>%s%s</body></html>\""
        ",\"volar\":\"<!DOCTYPE html><html> <head> <meta charset='utf-8'> <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script> <script src='/js/menu.js'></script> <link rel='stylesheet' href='/css/main.css'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <link href='https://fonts.googleapis.com/css?family=Roboto' rel='stylesheet'>%s</head> <body> <div class='header'> </div> <div id='menu'> Загрузка меню... </div> <main> <div id='ctx'>%s </div> </main> <footer> %s <p> Позвоните нам: 8 (499) 345-07-00 </p> <p> © 2014 by Volar </p> </footer> </body></html>\""
      "}";
  const std::string scripsStr =
        "{"
          "\"default\":\"<script>console.log('script enable!')</script>\","
          "\"default2\":\"<script>console.log('script2 enable!')</script>\","
          "\"volar\":\"\""
        "}";
  const std::string strJsonHeader =
        "["
          "\"<link rel='shortcut icon' href='/favicon.ico'>\","
          "\"<link href='/static/css/main.c0b27c9a.css' rel='stylesheet'>\""
        "]";

  Json::Value scripts = strToJSON(scripsStr);
  Json::Value tpls = strToJSON(tplsStr);
  std::cout << "tpls:" << tpls << '\n';
  std::cout << "bodyStr:" << bodyStr << '\n';
  Json::Value body = strToJSON(bodyStr);

  std::string scriptName = body["script"].asString();
  std::string tplName = body["tpl"].asString();
  std::string tpl = tpls[tplName].asString();
  std::string page = body["page"].asString();
  std::string ctx = body["ctx"].asString();
  std::string end = scripts[scriptName].asString();
  std::string toHeader = jsonStrArrayToStr(strJsonHeader);
  if (body.isMember("header"))
    toHeader += body["header"].asString();


  std::string html;
  if (createHTML(html, tpl, toHeader, ctx, end)) {
    saveFile (HTML_DIR + "/json", page+".json", bodyStr);
    saveFile (HTML_DIR, page+PAGE_SUFFIX, html);
  }
  std::string menuFile = MENU_FILE;
  menu(menuFile, body["id"].asString(),body["title"].asString());

  return 0;
}

int menu(std::string file, std::string id, std::string title) {
  // std::string title = "kkkkkk",
  //             id = "volar1";
  std::string ctx = "[{\"id\":\"index\",\"title\":\"Главная\"},{\"id\":\"sp\",\"title\":\"Сэндвич панели\"},{\"id\":\"ge\",\"title\":\"Доборные элементы\"},"
    "{\"id\":\"cd\",\"title\":\"Холодильные двери\"},{\"id\":\"osb\",\"title\":\"OSB сэндвич панели\"},{\"id\":\"sg\",\"title\":\"Скоростные ворота\"},"
    "{\"id\":\"DOORHAN\",\"title\":\"Секционные ворота и рольставни DOORHAN\"},{\"id\":\"contacts\",\"title\":\"Контакты\"},{\"id\":\"volar\",\"title\":\"TEST\"}]";

  // std::cout << "ctx:" << ctx << '\n';
  std::ifstream ifs(file);
  if (ifs.is_open())
    std::cout << "/* open */" << '\n';
  else return -1;
  // std::cout << "file:" << std::string(ifs) << '\n';
  std::cout << ifs.rdbuf() << '\n';
  Json::Reader reader;
  Json::Value obj;
  // reader.parse(ctx, obj);
  reader.parse(ctx, obj);
  ifs.close();

  int curIndex = -1;
  for ( int index = 0; index < obj.size(); ++index ) {
    if (obj[index]["id"].asString() == id) {
      curIndex = index;
    }
  }
  if (curIndex != -1) {
    obj[curIndex]["title"] = title;
  }
  else {
    Json::Value newItem;
    newItem["id"] = id;
    newItem["title"] = title;
    obj.append(newItem);
  }
  Json::FastWriter fastWriter;
  std::string output = fastWriter.write(obj);

  Json::FastWriter fast;
  std::string sFast = fast.write(obj);
  std::cout << "sFast:\t" << sFast << '\n';


  std::cout << "UTF-8: " << output << '\n';
  Json::StyledWriter styledWriter;
  // std::cout << ">> " << styledWriter.write(obj) << '\n';
  std::ofstream file_id;
  file_id.open(file.c_str());
  // file_id << styledWriter.write(obj);
  file_id << jsonToStr(obj);
  file_id.close();



  // Json::StyledWriter writer;
  // // Make a new JSON document for the configuration. Preserve original comments.
  // std::string outputConfig = writer.write( obj );
  // // std::cout << "outputConfig:" >> outputConfig << '\n';
  // std::cout << ">>>>>>>>>>" << jsonToStr(obj)<< '\n';
  // std::cout << ">>>>>>>>>>" << jsonToStr(obj)<< '\n';

  return 0;
}

int main () {
  setlocale(LC_ALL, "ru_RU.UTF-8");
  const std::string HTML_DIR = "/var/www/html";
  const std::string bodyStr =
        "{"
          "\"ctx\":\"<div class='el'><div class='desc-f'><p>TEST ''sadasdasdsad'''''''VOLAR'''''''''</p></div></div>\","
          "\"page\":\"volar\","
          "\"tpl\":\"volar\","
          "\"header\":\"<meta description:'test'><title>ООО \\\"Волар\\\"</title>\","
          "\"script\":\"volar\","
          "\"id\":\"testasdasdsad\","
          "\"title\":\"it's testasdasdasdasd\""
        "}";

  www(HTML_DIR, bodyStr);
  // std::string menuFile = "/var/www/html/json/menu.json";
  // menu(menuFile,"ID","TITLE что-то");

  return 0;
}
