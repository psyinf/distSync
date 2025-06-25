#include <distSync/distSync.hpp>
#include <iostream>
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
try
{
    // This is a placeholder for the client application main function.
    distSync::Server server;
    std::print("Starting distSync server...\n");
    server.run().get();

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