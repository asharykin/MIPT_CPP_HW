#include <iostream>
int main()
{
    std::cout << R"(They could describe // everything with "one" single word)" << std::endl;
    std::cout << R"^(

                 )^" << std::endl;
    std::cout << R"gnarly(/* Boba tea (Gnarly)
                           * 'Tesla' (Gnarly)
                           * Fried chicken (Gnarly)
                           */)gnarly" << std::endl;
}
