#include <uWS/uWS.h>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    uWS::Hub h;

    h.onConnection([](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest req) {
        std::cout << "Client connected" << std::endl;
    });

    h.onDisconnection([](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
        std::cout << "Client disconnected" << std::endl;
    });

    // Broadcast heartbeat every 5 seconds.
    std::thread([&h]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            h.getDefaultGroup<uWS::SERVER>().broadcast("Heartbeat: update from backend", uWS::OpCode::TEXT);
        }
    }).detach();

    if (h.listen(9002)) {
        std::cout << "WebSocket server started on ws://localhost:9002/ws" << std::endl;
        h.run();
    } else {
        std::cerr << "Failed to listen on port 9002" << std::endl;
    }
    return 0;
}
