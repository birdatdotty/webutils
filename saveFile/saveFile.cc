#include <fstream>

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

// #include <boost/locale.hpp>

#include <iostream>

bool saveFile (std::string myPath, std::string fileName, std::string data) {
  using namespace std;
  // using namespace boost::locale;
  // generator gen;
  // locale loc=gen("");
  int i;
  std::replace( myPath.begin(), myPath.end(), ' ', '_');

  path p =  path(myPath);
  path dirP = p;
  dirP/=path(fileName);
  path tmpDir;
  for (path::iterator it = dirP.begin(); it != dirP.end(); ++it) {
    tmpDir /= *it;
    if (*it == "..")
      return false;

    if (path(tmpDir) != dirP)
      boost::filesystem::create_directory(path(tmpDir));
  }

  p/=path(fileName);
  std::ofstream fd(p.string());
  if (fd.is_open()) {
    fd << data << std::endl;
    fd.close();
  } else
    return false;

  return true;
}
