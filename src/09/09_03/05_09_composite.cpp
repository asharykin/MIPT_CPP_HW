///////////////////////////////////////////////////////////////////////

// chapter : Software Design Patterns

///////////////////////////////////////////////////////////////////////

// section : Constructional Patterns

///////////////////////////////////////////////////////////////////////

// content : Pattern Composite

///////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <cstddef>
#include <memory>
#include <vector>

///////////////////////////////////////////////////////////////////////

class Entity
{
public :

    virtual ~Entity() = default;

//  -----------------------------

    virtual int test() const = 0;
};

///////////////////////////////////////////////////////////////////////

class Client : public Entity
{
public :

    int test() const override
    {
        return 1;
    }
};

///////////////////////////////////////////////////////////////////////

class Server : public Entity
{
public :

    int test() const override
    {
        return 2;
    }
};

///////////////////////////////////////////////////////////////////////

class Composite : public Entity
{
public :

    ~Composite() override = default;

//  ------------------------------------

    void add(std::shared_ptr<Entity> entity)
    {
        m_entities.push_back(std::move(entity));
    }

//  ------------------------------------

    int test() const override
    {
        auto x = 0;

        for (const auto& entity : m_entities)
        {
            if (entity)
            {
                x += entity->test();
            }
        }

        return x;
    }

private :

    std::vector<std::shared_ptr<Entity>> m_entities;
};

///////////////////////////////////////////////////////////////////////

auto make_composite(std::size_t size_1, std::size_t size_2) -> std::shared_ptr<Entity>
{
    auto composite = std::make_shared<Composite>();

    for (auto i = 0uz; i < size_1; ++i) { composite->add(std::make_shared<Client>()); }

    for (auto i = 0uz; i < size_2; ++i) { composite->add(std::make_shared<Server>()); }

    return composite;
}

///////////////////////////////////////////////////////////////////////

TEST(CompositePatternTest, TotalSumCalculation)
{
    auto composite = std::make_shared<Composite>();

    for (auto i = 0uz; i < 5; ++i)
    {
        composite->add(make_composite(1, 1));
    }

    std::shared_ptr<Entity> entity = composite;

    EXPECT_EQ(entity->test(), 15);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

///////////////////////////////////////////////////////////////////////
