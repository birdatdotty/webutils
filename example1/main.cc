#include <iostream>
#include <string>
#include <CServer.h>

#include "proxy/proxy.h"


int main(int argc, char* argv[]) {
    CServer s;

    std::string docroot;
    uint16_t port = 9000;

    if (argc == 1) {
        docroot = "/var/www/html";
    }

    if (argc >= 2) {
        docroot = std::string(argv[1]);
    }

    if (argc >= 3) {
        int i = atoi(argv[2]);
        if (i <= 0 || i > 65535) {
            std::cerr << "invalid port" << std::endl;
            return 1;
        }

        port = uint16_t(i);
    }
    chdir(docroot.c_str());
    proxy *p = new proxy();
    s.setCallback(p);

    s.run(docroot, port);
    return 0;
}
