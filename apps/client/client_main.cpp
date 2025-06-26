#include <distSync/distSync.hpp>
#include <iostream>
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    // This is a placeholder for the client application main function.
    //distSync::Client client("192.168.178.152");
    distSync::Client client("localhost");

    auto diff = client.sync();
    std::cout << "Initial Time difference with server: " << diff << " nanoseconds" << std::endl;

    while (true)
    {
        // Simulate some work
        std::this_thread::sleep_for(std::chrono::seconds(1));
        // Get the current time difference
        auto synced_time = client.getTimeBase();
        std::println("Current synced time: {}", synced_time / 1'000'000'000.0f);
        
    }
    // system clock cast from synced time

    // set steady clock

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