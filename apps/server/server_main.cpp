#include <distSync/distSync.hpp>
#include <iostream>
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    // This is a placeholder for the client application main function.
    distSync::Server server;
    std::print("Starting distSync server...\n");
    auto f = server.run();

    while (true)
    {
        std::println("Local ref time {}", server.getLocalRefTimeSinceEpoch_ns(std::chrono::high_resolution_clock::now()) / 1'000'000'000.0f );
        std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // Sleep for 1 second
    }
    f.get(); // Wait for the server to finish running
    return 0;
}
catch (const std::exception& e)
{
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
}
catch (...)
{
    std::cerr << "Unknown exception occurred." << std::endl;
    return 1;
}