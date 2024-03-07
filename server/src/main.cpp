#include <nlohmann/json.hpp>
#include <zmq.hpp>

#include <iostream>
#include <thread>
#include <signal.h>

using json = nlohmann::json;

static auto is_running_ = true;
// shudown the server
auto signal_handler(int signal) -> void
{
    std::cout << "Shutting down the server..." << std::endl;
    is_running_ = false;
}

auto is_running() -> bool
{
    return is_running_;
}

struct InComingData {
    std::string name;
};

struct OutGoingData {
    std::string name;
};

namespace nlohmann {
template <>
struct adl_serializer<InComingData> {
    static auto from_json(const json& j) -> InComingData { return InComingData{j.at("Name").get<std::string>()}; }
    static auto to_json(json& j, const InComingData& p) -> void { j = json{{"Name", p.name}}; }
};

template <>
struct adl_serializer<OutGoingData> {
    static auto from_json(const json& j) -> OutGoingData { return OutGoingData{j.at("Name").get<std::string>()}; }
    static auto to_json(json& j, const OutGoingData& p) -> void { j = json{{"Name", p.name}}; }
};
}  // namespace nlohmann

auto server_service(zmq::context_t& context) -> void
{
    zmq::socket_t socket(context, zmq::socket_type::rep);
    socket.bind("tcp://*:5555");
    auto timeout = std::chrono::milliseconds{1000};
    socket.set(zmq::sockopt::rcvtimeo, static_cast<int>(timeout.count()));

    while (is_running()) {
        try {
            auto request      = zmq::message_t{};
            const auto result = socket.recv(request, zmq::recv_flags::none);
            if (!result.has_value()) {
                std::cerr << "[Timeout] receiving incoming data" << std::endl;
                continue;
            }
            const auto deserialized = json::from_msgpack(request.to_string());
            const auto incoming     = deserialized.get<InComingData>();

            std::cout << "Received request: " << incoming.name << std::endl;

            // // send reply with echo
            const auto outgoing   = OutGoingData{incoming.name};
            const auto serialized = json::to_msgpack(outgoing);
            auto reply            = zmq::message_t(serialized.begin(), serialized.end());
            socket.send(reply, zmq::send_flags::none);
        }
        catch (const zmq::error_t& e) {
            std::cerr << "zmq error process: " << e.what() << "\n";
            continue;
        }
        catch (const std::exception& e) {
            std::cerr << "Fail processing request: " << e.what() << "\n";
            continue;
        }
        catch (...) {
            std::cerr << "Fail processing request: unknown error\n";
            continue;
        }
    }
}

int main(int argc, const char** argv)
{
    // register signal handler
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    std::cout << "Start server..." << std::endl;

    // create service with zmq
    // server service pattern
    auto context = zmq::context_t{};
    try {
        std::thread server_thread(server_service, std::ref(context));
        server_thread.join();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return EXIT_SUCCESS;
}
