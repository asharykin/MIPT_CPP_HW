/////////////////////////////////////////////

// chapter : Software Design Patterns

/////////////////////////////////////////////

// section : Generative Patterns

/////////////////////////////////////////////

// content : Pattern Abstract Factory

/////////////////////////////////////////////

#include <memory>

class Entity
{
public :

    virtual ~Entity() = default;
};

/////////////////////////////////////////////

class Client : public Entity {};

class Server : public Entity {};

/////////////////////////////////////////////

class Factory
{
public :

    virtual ~Factory() = default;

    //  -----------------------------------------

    virtual std::unique_ptr<Entity> make_entity() const = 0;
};

/////////////////////////////////////////////

class Factory_Client : public Factory
{
public :

    std::unique_ptr<Entity> make_entity() const override
    {
        return std::make_unique<Client>();
    }
};

/////////////////////////////////////////////

class Factory_Server : public Factory
{
public :

    std::unique_ptr<Entity> make_entity() const override
    {
        return std::make_unique<Server>();
    }
};

/////////////////////////////////////////////

int main()
{
    std::unique_ptr<Factory> factory = std::make_unique<Factory_Client>();

    //  ---------------------------------

    std::unique_ptr<Entity> entity = factory->make_entity();
    // std::shared_ptr<Entity> will work as well

    //  ---------------------------------
}

/////////////////////////////////////////////