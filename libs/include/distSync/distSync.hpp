#pragma once
#include <chrono>
#include <future>
#include <print>
#include <stop_token>
#include <string>

#include <zmq.hpp>

/*
Extremely simple distributed synchronization library using ZeroMQ.
This library provides a basic server that listens for requests and responds with the current time in nanoseconds since
epoch to allow the client to synchronize its clock with the server or keep track of time differences.

A more elaborate implementation could include features like:
- Client gets initial time from server
- Client synchronizes its clock with the server
- Client sends its current time to the server again, server calculates the difference and sends it back to the client
*/
namespace distSync {
class Server
{
public:
    Server() = default;

    ~Server() { _stopSource.request_stop(); }

    std::future<void> run()
    {
        return std::async(std::launch::async, [this]() {
            // Server logic goes here
            zmq::socket_t socket(_context, zmq::socket_type::rep);
            socket.bind("tcp://*:33337");

            while (!_stopSource.stop_requested())
            {
                zmq::message_t request;
                std::ignore = socket.recv(request, zmq::recv_flags::none);
                std::print("Received request: {}\n", request.to_string());
                // get current time
                auto now = std::chrono::high_resolution_clock::now();
                auto ns_since_epoch =
                    std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
                auto ns_since_epoch_str = std::to_string(ns_since_epoch);

                zmq::message_t response(ns_since_epoch_str);
                socket.send(response, zmq::send_flags::none);
            }
        });
    }

private:
    std::stop_source _stopSource;
    zmq::context_t   _context{1};
};

class Client
{
public:
    Client(std::string remote_address = "localhost") { _socket.connect("tcp://" + remote_address + ":33337"); }

    ~Client() = default;

    std::chrono::high_resolution_clock::time_point synced_time() const
    {
        // now - start + timeDifference
        auto now = std::chrono::high_resolution_clock::now();
        auto diff = now - _start;
        return std::chrono::high_resolution_clock::time_point(
            std::chrono::nanoseconds(diff.count() + _timeDifference_ns));
    }

    auto sync() -> std::int64_t

    {
        zmq::message_t request(0);
        _socket.send(request, zmq::send_flags::none);
        zmq::message_t response;
        std::ignore = _socket.recv(response, zmq::recv_flags::none);
        // parse response
        auto ns_since_epoch = std::stoll(response.to_string());
        _start = std::chrono::high_resolution_clock::now();
        auto server_time = std::chrono::high_resolution_clock::time_point(std::chrono::nanoseconds(ns_since_epoch));

        _timeDifference_ns = _start.time_since_epoch().count() - server_time.time_since_epoch().count();
        return _timeDifference_ns;
    }

private:
    std::int64_t                                   _timeDifference_ns;
    std::chrono::high_resolution_clock::time_point _start;
    zmq::context_t                                 _context{1};
    zmq::socket_t                                  _socket{_context, zmq::socket_type::req};
};
} // namespace distSync