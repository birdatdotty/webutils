#include "CServer.h"

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include "boost/bind.hpp"

#include <fstream>
#include <iostream>
#include <set>
#include <streambuf>
#include <string>


using std::cout;
using std::endl;



void returnMessage(websocketpp::server<websocketpp::config::asio>* m_endpoint,
    websocketpp::connection_hdl* hdl, std::string answer,
    websocketpp::server<websocketpp::config::asio>::message_ptr msg) {
  try {
      m_endpoint->send(*hdl, answer ,websocketpp::frame::opcode::text);
      // m_endpoint->send(*hdl, msg->get_payload(), msg->get_opcode());
  } catch (const websocketpp::lib::error_code& e) {
      std::cout << "Echo failed because: " << e
                << "(" << e.message() << ")" << std::endl;
  }

}

void CServer::on_message(websocketpp::connection_hdl hdl, websocketpp::server<websocketpp::config::asio>::message_ptr msg) {
    if (msg->get_payload() == "stop-listening") {
        m_endpoint.stop_listening();
        return;
    }
    // sendAll("TEST allMessage");
    auto retClient = boost::bind(returnMessage, &m_endpoint, &hdl, boost::placeholders::_1, msg);

    std::string json = msg->get_payload();

    cb->getMessage(json, retClient, m_connections[hdl]);
}

void CServer::setCallback(Callback* callback){
  // cout << "CServer::setCallback" << endl;
  cb = callback;

  auto retSendAll = boost::bind(&CServer::sendAll, this, boost::placeholders::_1);
  cb->setMessageAll(retSendAll);
  // callback("TEST callback");
  // cb("TEST callback");
  // cout << "CServer::setCallback" << endl;
}

void CServer::sendAll (std::string msg) {
  for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
    // std::cout << ">>  it->second->getName()" << it->second->getName() << '\n';
    if (it->second->init)
      m_endpoint.send((*it).first,msg.c_str(),websocketpp::frame::opcode::text);
  }
}

void CServer::on_http(connection_hdl hdl) {
    // Upgrade our connection handle to a full connection_ptr
    server::connection_ptr con = m_endpoint.get_con_from_hdl(hdl);

    std::ifstream file;
    std::string filename = con->get_resource();
    std::string response;

    std::cout << "get_host (): " << con->get_host ()  << '\n';
    std::cout << "get_resource (): " << con->get_resource ()  << '\n';
    // std::cout << "get_origin (): " << con->get_origin ()  << '\n';
    std::cout << "get_local_close_reason (): " << con->get_local_close_reason ()  << '\n';
    std::cout << "get_origin (): " << con->get_remote_close_reason ()  << '\n';

    m_endpoint.get_alog().write(websocketpp::log::alevel::app,
        "http request1: "+filename);

    m_endpoint.get_alog().write(websocketpp::log::alevel::app,
        "http request2: "+filename);

    con->set_body(cb->xhr( filename, con->get_request_body() ) );
    con->set_status(websocketpp::http::status_code::ok);
}

void CServer::on_open(connection_hdl hdl) {
    std::pair<connection_hdl,Account*> in =
     std::make_pair(hdl,new Account());

    m_connections.insert( in );
    // m_connections.insert(hdl);
}
void CServer::on_close(connection_hdl hdl) {
    m_connections.erase(hdl);
}
void CServer::run(std::string docroot, uint16_t port) {
    std::stringstream ss;
    ss << "Running telemetry server on port __ "<< port <<" using docroot=" << docroot;
    m_endpoint.get_alog().write(websocketpp::log::alevel::app,ss.str());

    m_docroot = docroot;

    // listen on specified port
    m_endpoint.listen(port);

    // Start the server accept loop
    m_endpoint.start_accept();

    // Start the ASIO io_service run loop
    try {
        m_endpoint.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}
CServer::CServer() {
    // set up access channels to only log interesting things
    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.set_access_channels(websocketpp::log::alevel::access_core);
    m_endpoint.set_access_channels(websocketpp::log::alevel::app);

    // Initialize the Asio transport policy
    m_endpoint.init_asio();

    // Bind the handlers we are using
    using websocketpp::lib::placeholders::_1;
    using websocketpp::lib::placeholders::_2;
    using websocketpp::lib::bind;
    m_endpoint.set_open_handler     (bind(&CServer::on_open,this,_1));
    m_endpoint.set_close_handler    (bind(&CServer::on_close,this,_1));
    m_endpoint.set_http_handler     (bind(&CServer::on_http,this,_1));
    // m_endpoint.set_message_handler  (bind(&CServer::on_message,this,_1,_2));
    m_endpoint.set_message_handler  (bind(&CServer::on_message,this,_1,_2));
}
