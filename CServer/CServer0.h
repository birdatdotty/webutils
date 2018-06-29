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
#include "Callback.h"
#include "Reducer.h"

class CServer {
public:
    typedef websocketpp::connection_hdl connection_hdl;
    typedef websocketpp::server<websocketpp::config::asio> server;
    CServer();

    void setReducerCreate (std::function<void*()>);
    void setXhr (std::function<std::string(std::string file,std::string body)> funct);
    void setWs  (std::function<
                  std::string(
                    std::string,
                    std::string,
                    void*,
                    std::function<void(std::string)>,
                    std::function<void(std::function<bool(void*)>,std::string)>
                  )> funct);
    void setCallback(Callback*);
    void run(std::string docroot, uint16_t port);
    void on_http(connection_hdl hdl);
    void on_open(connection_hdl hdl);
    void on_close(connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg);
private:
    typedef std::map<connection_hdl, void*, std::owner_less<connection_hdl>> con_list;
    void sendAll (std::function<bool(void*)>,std::string msg);

    std::function<void*()> reducerCreate;
    std::function<std::string(std::string,std::string)> xhrFunct;
    std::function<
                  void (std::string,
                        std::string,
                        void*,
                        std::function<void(std::string)>,
                        std::function<void(std::function<bool(void*)>,std::string)>)
                  > wsFunct;
    server m_endpoint;
    con_list m_connections;
    server::timer_ptr m_timer;
    std::string m_docroot;
};

#endif
