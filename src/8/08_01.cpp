#include <gtest/gtest.h>

class Entity_v1
{
public:
    Entity_v1(int value) : m_value_private(value) { }

    int get_value() const
    {
        return m_value_private;
    }

private:
    int m_value_private;
};

class Entity_v2
{
public:
    int m_value_public;
};

union Entity_Union
{
    Entity_v1* p_e1;
    Entity_v2* p_e2;

    Entity_Union(Entity_v1* target)
    {
        p_e1 = target;
    }
};

class EntityTest : public ::testing::Test
{
protected:
    Entity_v1 entity{50};
};

TEST_F(EntityTest, InitialValueIsCorrect)
{
    EXPECT_EQ(entity.get_value(), 50);
}

TEST_F(EntityTest, ModifyViaReinterpretCast)
{
    reinterpret_cast<Entity_v2&>(entity).m_value_public = 100;
    EXPECT_EQ(entity.get_value(), 100);
}

TEST_F(EntityTest, ModifyViaUnion)
{
    Entity_Union entity_union(&entity);
    entity_union.p_e2->m_value_public = 200;
    EXPECT_EQ(entity.get_value(), 200);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
