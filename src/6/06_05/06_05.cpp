#include <iostream>
#include <boost/dll.hpp>

int main() {
    std::string version;
    std::cout << "Enter library version (v1/v2): ";
    std::cin >> version;

    std::string path;

    if (version == "v1") {
        path = "libtest_v1/libtest_v1.so";
    } else if (version == "v2") {
        path = "libtest_v2/libtest_v2.so";
    } else {
        std::cerr << "invalid version!" << std::endl;
        return 1;
    }

    auto test_function = boost::dll::import_symbol<void()>(path, "test");
    test_function();
}
