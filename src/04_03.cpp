#include <iostream>
#include <vector>

template<typename Container> void handle(Container& container, int arg)
{
    container.push_back(arg);
}

template<typename Container, typename T> void handle(Container& container, T arg) { }

template<typename Container, typename ... Ts> void handle(Container& container, Ts ... args)
{
    (handle(container, args), ...);
}

int main() {
    std::vector<int> vector;
    handle(vector, 1, 2.0, 3, 3.5, 4, 5);
    for (auto item : vector)
    {
        std::cout << item << " ";
    }
}
