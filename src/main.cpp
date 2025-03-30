#include "websocket_client.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: websocket_client <host> <port>\n"
                  << "Example:\n"
                  << " websocket_client echo.websocket.org 80\n";
        return EXIT_FAILURE;
    }

    std::string host = argv[1];
    std::string port = argv[2];

    try {
        WebSocketClient client(host, port);
        std::string message;

        std::cout << "Received: " << client.receive() << std::endl;

        while (true) {
            std::cout << "> ";
            std::getline(std::cin, message);

            if (message == "exit") break;

            client.send(message);
            std::cout << "Received: " << client.receive() << std::endl;
        }

        client.close();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}
