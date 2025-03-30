#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <iostream>

namespace net       = boost::asio;
namespace ssl       = net::ssl;
namespace beast     = boost::beast;
namespace websocket = beast::websocket;

using tcp = net::ip::tcp;

class WebSocketClient {
  public:
    WebSocketClient(const std::string& host, const std::string& port);
    void send(const std::string& message);
    std::string receive();
    bool is_open() const;
    void close();

  private:
    void connect(const std::string& host, const std::string& port);

    net::io_context    ioc;
    ssl::context       ctx;
    tcp::resolver      resolver;
    websocket::stream<beast::ssl_stream<tcp::socket>> ws;
};

#endif // WEBSOCKET_CLIENT_H

