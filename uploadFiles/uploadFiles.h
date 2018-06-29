#ifndef _UPLOAD_FILES_
#define _UPLOAD_FILES_
#include <json/json.h>
extern bool(*saveFile) (std::string myPath,               std::string fileName,               std::string data);

bool uploadFiles(std::string to, Json::Value data);
#endif
