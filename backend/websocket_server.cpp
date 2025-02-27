#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib> // For popen
#include <csignal> // For signal handling
#include <nlohmann/json.hpp>

// Use WebSocket++ namespace
typedef websocketpp::server<websocketpp::config::asio> server;

// Global flag for graceful shutdown
bool running = true;

/**
 * Helper function to execute shell commands and capture output.
 */
std::string execShellCommand(const std::string& cmd) {
    std::array<char, 256> buffer;
    std::string result;
    std::unique_ptr<FILE, int(*)(FILE*)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("Failed to execute command: " + cmd);
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

/**
 * Function to detect all attached hard drives using `lsblk`.
 */
std::vector<std::map<std::string, std::string>> getAttachedDrives() {
    std::vector<std::map<std::string, std::string>> drives;
    try {
        // Execute lsblk command to get basic drive info
        std::string output = execShellCommand("lsblk -d -o NAME,SIZE,TYPE -J");
        nlohmann::json drivesJson = nlohmann::json::parse(output);

        // Filter only physical drives (type == "disk")
        for (const auto& drive : drivesJson["blockdevices"]) {
            std::string type = drive.value("type", "");
            if (type == "disk") { // Include only physical drives
                std::string name = drive.value("name", "Unknown");
                std::string size = drive.value("size", "Unknown");

                // Add drive info to the list
                drives.push_back({
                    {"name", name},
                    {"capacity", size}
                });
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error detecting drives: " << e.what() << std::endl;
    }
    return drives;
}

/**
 * WebSocket message handler.
 */
void on_message(server* s, websocketpp::connection_hdl hdl, server::message_ptr msg) {
    try {
        // Log the received message
        std::cout << "Message received: " << msg->get_payload() << std::endl;

        // Fetch attached drives
        auto drives = getAttachedDrives();

        // Prepare JSON response
        nlohmann::json jsonResponse;
        if (drives.empty()) {
            jsonResponse["error"] = "No drives detected";
        } else {
            jsonResponse["drives"] = drives;
        }

        // Log the response
        std::cout << "Sending JSON response: " << jsonResponse.dump(4) << std::endl;

        // Send the response to the client
        s->send(hdl, jsonResponse.dump(), msg->get_opcode());
    } catch (const std::exception& e) {
        // Log any errors
        std::cerr << "Error handling message: " << e.what() << std::endl;
    }
}

/**
 * Signal handler for graceful shutdown.
 */
void handle_signal(int signal) {
    std::cout << "Received signal " << signal << ". Shutting down gracefully..." << std::endl;
    running = false;
}

int main() {
    // Set up signal handling for graceful shutdown
    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    // Create WebSocket server
    server ws_server;
    ws_server.set_access_channels(websocketpp::log::alevel::all);
    ws_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
    ws_server.init_asio();

    // Register message handler
    ws_server.set_message_handler(std::bind(&on_message, &ws_server, std::placeholders::_1, std::placeholders::_2));

    // Listen on port 9002
    unsigned short port = 9002;
    ws_server.listen(port);
    ws_server.start_accept();
    std::cout << "WebSocket server started on ws://localhost:" << port << std::endl;

    // Run the server loop
    while (running) {
        ws_server.poll_one();
    }

    // Stop the server gracefully
    ws_server.stop_listening();
    std::cout << "WebSocket server stopped." << std::endl;

    return 0;
}
//test