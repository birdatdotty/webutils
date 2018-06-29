#include <iostream>
#include "pg.h"
#include <vector>
#include "helperJson.h"

using namespace std;

int main() {
  std::cout << "Hello!" << '\n';
  Cpg a("testdb", "postgres", "fgtkmcby", "127.0.0.1",  5432);
  a.open();
  a.createTable();
  std::vector<string> v = a.getProjects();

  // insertInfo("CrEaToR");
  // a.insertInfoPost("CrEaToR", "{\"post\":\"kj aaaaaaaaaaaaaaaaaaaaaaaajhhj\"}");
  // a.insertInfoPost("CrEaToR", "{\"post\":\"kj jhhj\"}");

  Json::Value preAnswer;
  // for (auto i: v)
  //     preAnswer[i] = strToJSON(a.getInfoPosts(i));
  //
  // std::cout << "getLoadPrj: " << getLoadPrj(v) << '\n';
  // std::cout << "getLoadTask: " << getLoadTask(preAnswer,v[0]) << '\n';

  // std::cout << "info:\n" << a.getInfoPosts() << '\n';



  // a.insertAchivements("sche2211","{\"imgs\":"
  //                     "[\"https://avatars.mds.yandex.net/get-banana/28825/x25CFcSEFGmeeOmAoHLpFvS8W_banana_20161021_unlim.png/optimize\","
  //                     "\"https://avatars.mds.yandex.net/get-banana/68433/x25YkaSX0YQx14IEmyBp3tKP_banana_20161021_BannerID_4353284033.png/optimize\"]"
  //                     ",\"text\":\"<p>lklklklklkkllkklklklkllkkl</p>\",\"people\":\"gluck\"}");
  // a.insertTeacher("Архангельская Светлана Александровна","{\"lessons\":[\"Русский язык\",\"Литература\"],\"desc\":\"\",\"people\":\"Архангельская Светлана Александровна\"}");
  // a.insertTeachers("[	{\"lessons\":[\"Математика\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Ялунина Светлана Станиславовна\"},{\"lessons\":[\"Английский язык\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"Классный руководитель/т. +79267226646\",\"people\":\"Дегтярева Юлия Валерьевна\"},	{\"lessons\":[\"Английский язык\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"т. +79252947465/e-mail: chekmareva.75@mail.ru\",\"people\":\"Чекмарева Наталья Валериевна\"},	{\"lessons\":[\"Русский язык\", \"литература\"],\"img\":\"/avatarts/Архангельская_Светлана_Александровна.png\",\"desc\":\"\",\"people\":\"Архангельская Светлана Александровна\"},	{\"lessons\":[\"География\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Алексеева Ирина Николаевна\"},	{\"lessons\":[\"ИЗО\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Терёхина Татьяна Александровна\"},	{\"lessons\":[\"Всеобщая история\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Димчогло Наталья Ивановна\"},{\"lessons\":[\"Французский язык\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Горшкова Елена Витальевна\"},	{\"lessons\":[\"Немецкий язык\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Бровкина Ирина Владимировна\"},	{\"lessons\":[\"Физическая культура\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Сидоркин Дмитрий Олегович\"},	{\"lessons\":[\"ОДНКР\"],\"img\":\"/avatarts/Николаенко_Ирина_Александровна.png\",\"desc\":\"\",\"people\":\"Николаенко Ирина Александровна\"},	{\"lessons\":[\"Биология\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Наливкина Ирина Владимировна\"},	{\"lessons\":[\"Технология (девочки)\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Лучанинова Анастасия Викторовна\"},	{\"lessons\":[\"Технология (мальчики)\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"т. +79265440313\",\"people\":\"Шатайло Петр Алексеевич\"},	{\"lessons\":[\"Обществознание\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"\",\"people\":\"Гаврилов Андрей Яковлевич\"},{\"lessons\":[\"Музыка\"],\"img\":\"/avatarts/unknown_user.jpg\",\"desc\":\"т. +79362345678\",\"people\":\"Торубарова Надежда Владимировна\"}]");
  std::cout << ">>>> " << a.getAchivements() << '\n';


// // insertInfoPost
//   std::string insert = "{\"posts\": [{\"ctx\": \"aasdkljlk lkadj lkajdf lkadj lkj\", \"time\": \"10.10\"}], \"creater\": {\"name\": \"infoPost\", \"time\": \"10.10\"}}";
//   Json::Value jsonInsert = strToJSON(insert);
//   std::cout << getCreater(jsonInsert) << '\n';
//   std::string creater = getCreater(jsonInsert);
//   a.insertInfoPost(creater, insert);

// // appendInfoPost
//   std::string insert = "{\"posts\": [{\"ctx\": \"aasdkljlk lkadj lkajdf lkadj lkj\", \"time\": \"10.10\"}], \"creater\": {\"name\": \"infoPost\", \"time\": \"10.10\"}}";
//   Json::Value jsonInsert = strToJSON(insert);
//   std::string creater = getCreater(jsonInsert);
//   a.appendInfoPost("21","{\"ctx\": \"aasdkljlk lkadj lkajdf lkadj lkj\", \"time\": \"10.10\"}");

  return 0;
}
