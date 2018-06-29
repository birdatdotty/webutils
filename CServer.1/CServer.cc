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

void CServer::on_message(websocketpp::connection_hdl hdl,
  websocketpp::server<websocketpp::config::asio>::message_ptr msg)
{
    if (msg->get_payload() == "stop-listening") {
        m_endpoint.stop_listening();
        return;
    }
    // sendAll("TEST allMessage");
    auto retClient = boost::bind(returnMessage, &m_endpoint, &hdl, boost::placeholders::_1, msg);
    wsFunct(m_endpoint.get_con_from_hdl(hdl)->get_resource(),
            msg->get_payload(),
            m_connections[hdl],
            retClient,
            boost::bind(&CServer::sendAll, this, boost::placeholders::_1, boost::placeholders::_2)
          );
}

void CServer::sendAll (std::function<bool(void*)> test1, std::string msg) {
  for (auto it = m_connections.begin(); it != m_connections.end(); ++it) {
    // std::cout << ">>  it->second->getName()" << it->second->getName() << '\n';
    if (test1(it->second)) {
      std::cout << "if (test1(it->second))!" << '\n';
      m_endpoint.send((*it).first,msg.c_str(),websocketpp::frame::opcode::text);
    }
  }
}

void CServer::on_http(connection_hdl hdl) {
    server::connection_ptr con = m_endpoint.get_con_from_hdl(hdl);

    std::string filename = con->get_resource();
    std::string response;

    con->set_body(xhrFunct(filename, con->get_request_body()) );
    con->set_status(websocketpp::http::status_code::ok);
    con->append_header("Content-type", "*");
    con->append_header("Access-Control-Allow-Origin", "*");
}

void CServer::on_open(connection_hdl hdl) {
  void* reducer = new Reducer();
    std::pair<connection_hdl,void*> in =
      std::make_pair(hdl, reducerCreate());

    m_connections.insert( in );
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

void CServer::setReducerCreate (std::function<void*()> func) {
  reducerCreate = func;
}

void CServer::setXhr (std::function<std::string(std::string file, std::string body)> func) {
  xhrFunct = func;
}

void CServer::setWs  (std::function<std::string(
                                      std::string,
                                      std::string,
                                      void*,
                                      std::function<void(std::string)>,
                                      std::function<void(std::function<bool(void*)>,std::string)>
                                    )> funct)
{
  wsFunct = funct;
}

CServer::CServer() {

  reducerCreate = [] () {return (void*)NULL;};
  // set up access channels to only log interesting things
  // m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
  // m_endpoint.set_access_channels(websocketpp::log::alevel::access_core);
  // m_endpoint.set_access_channels(websocketpp::log::alevel::app);
  m_endpoint.clear_access_channels(websocketpp::log::alevel::none);
  // m_endpoint.set_access_channels(websocketpp::log::alevel::access_core);
  m_endpoint.set_access_channels(websocketpp::log::alevel::none);


  // Initialize the Asio transport policy
  m_endpoint.init_asio();

  // Bind the handlers we are using
  using websocketpp::lib::placeholders::_1;
  using websocketpp::lib::placeholders::_2;
  using websocketpp::lib::bind;
  m_endpoint.set_open_handler     (bind(&CServer::on_open,this,_1));
  m_endpoint.set_close_handler    (bind(&CServer::on_close,this,_1));
  m_endpoint.set_http_handler     (bind(&CServer::on_http,this,_1));
  m_endpoint.set_message_handler  (bind(&CServer::on_message,this,_1,_2));
}
