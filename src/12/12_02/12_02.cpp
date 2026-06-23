#include <cstdio>

int main()
{
    const char* s = "#include <cstdio>%1$c%1$cint main()%1$c{%1$c    const char* s = %2$c%3$s%2$c;%1$c    std::printf(s, 10, 34, s);%1$c}%1$c";
    std::printf(s, 10, 34, s);
}
