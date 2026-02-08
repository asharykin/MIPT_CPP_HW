#include <cassert>
#include <iostream>

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

union Entity_Union {
    Entity_v1* p_e1;
    Entity_v2* p_e2;

    Entity_Union(Entity_v1* target) {
        p_e1 = target;
    }
};

int main()
{
    Entity_v1 entity(50);
    assert(entity.get_value() == 50);

    reinterpret_cast<Entity_v2&>(entity).m_value_public = 100;
    assert(entity.get_value() == 100);

    Entity_Union entity_union(&entity);
    entity_union.p_e2->m_value_public = 200;
    assert(entity.get_value() == 200);
}
