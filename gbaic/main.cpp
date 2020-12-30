#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    try
    {
        throw std::runtime_error("YIKES");
        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << argv[0] << ": " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
