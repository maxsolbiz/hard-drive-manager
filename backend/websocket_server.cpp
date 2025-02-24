// backend/websocket_server.cpp
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <functional> // Include this for std::bind and std::placeholders

typedef websocketpp::server<websocketpp::config::asio> server;

void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    std::cout << "Message received: " << msg->get_payload() << std::endl;
    s->send(hdl, "Message received!", msg->get_opcode());
}

int main() {
    server ws_server;

    // Use std::placeholders::_1 and std::placeholders::_2
    ws_server.set_message_handler(std::bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));
    ws_server.init_asio();
    ws_server.listen(9002);
    ws_server.start_accept();

    std::cout << "WebSocket server started on port 9002..." << std::endl;
    ws_server.run();
    return 0;
}