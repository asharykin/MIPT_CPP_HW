#include <cassert>
#include <iostream>

class Client
{
public :
    std::string test() const
    {
        return "Client::test";
    }
};

class Server
{
public :

     std::string test() const
    {
        return "Server::test";
    }
};

template<typename S> class Entity : public S
{
public :

    std::string test() const
    {
        return S::test();
    }
};

int main()
{
    Entity<Client> e1;
    Entity<Server> e2;
    assert(e1.test() == "Client::test");
    assert(e2.test() == "Server::test");
}