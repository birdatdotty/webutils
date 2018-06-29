#ifndef _CSERVER_H_
#define _CSERVER_H_

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include <fstream>
#include <iostream>
#include <set>
#include <map>
#include <streambuf>
#include <string>
#include <functional>
// #include "proxy.h"
#include "./Callback.h"
#include "./account.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

// typedef std::string (*callback)(std::string);

class CServer {
public:
    typedef websocketpp::connection_hdl connection_hdl;
    typedef websocketpp::server<websocketpp::config::asio> server;

    CServer();
    void setCallback(Callback*);
    void run(std::string docroot, uint16_t port);
    void on_http(connection_hdl hdl);
    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg);
    // void on_message(websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2);
    // void on_message(&on_message, this, websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg);
private:
    typedef std::map<connection_hdl, Account*, std::owner_less<connection_hdl>> con_list2;

    // typedef std::map<<connection_hdl,std::owner_less<connection_hdl>>, Account*> con_list2;
    typedef std::set<
              connection_hdl,
              std::owner_less<
                connection_hdl
              >
            > con_list;
    void sendAll (std::string msg);

    Callback* cb;
    server m_endpoint;
    con_list2 m_connections;
    server::timer_ptr m_timer;
    std::string m_docroot;
};

#endif
