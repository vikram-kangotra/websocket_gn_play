#include "websocket_client.h"

WebSocketClient::WebSocketClient(const std::string& host, const std::string& port)
    : ctx(ssl::context::tlsv12_client), ws(ioc, ctx), resolver(ioc)
{
    ctx.set_default_verify_paths();  // Ensure SSL certificates are validated
    connect(host, port);
}

void WebSocketClient::connect(const std::string& host, const std::string& port)
{
    try {
        // Resolve DNS
        auto const results = resolver.resolve(host, port);

        // Connect TCP layer
        net::connect(ws.next_layer().next_layer(), results.begin(), results.end());

        // Set SNI hostname
        if (!SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str())) {
            throw beast::system_error{
                {static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()},
                "Failed to set SNI Hostname"
            };
        }

        // Perform SSL handshake
        ws.next_layer().handshake(ssl::stream_base::client);

        // Perform WebSocket handshake
        ws.handshake(host, "/");
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        throw;
    }
}

void WebSocketClient::send(const std::string& message)
{
    try {
        ws.write(net::buffer(message));
    } catch (const std::exception& e) {
        std::cerr << "Send error: " << e.what() << std::endl;
    }
}

std::string WebSocketClient::receive()
{
    try {
        beast::flat_buffer buffer;
        ws.read(buffer);
        return beast::buffers_to_string(buffer.data());
    } catch (const std::exception& e) {
        std::cerr << "Receive error: " << e.what() << std::endl;
        return "";
    }
}

bool WebSocketClient::is_open() const
{
    return ws.is_open();
}

void WebSocketClient::close()
{
    try {
        ws.close(websocket::close_code::normal);
    } catch (const std::exception& e) {
        std::cerr << "Close error: " << e.what() << std::endl;
    }
}
