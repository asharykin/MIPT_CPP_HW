#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>

class GameObject
{
public:
    virtual ~GameObject() = default;

    virtual void info() = 0;
    virtual void performAction() = 0;
};

class Unit : public GameObject
{
public:
    void performAction() override
    {
        attack();
        move();
        defend();
    }

protected:
    virtual void attack() = 0;
    virtual void move()
    {
        std::cout << "Unit moves." << std::endl;
    }

    virtual void defend()
    {
        std::cout << "Unit defends itself." << std::endl;
    }
};

class Archer : public Unit
{
public:
    void info() override
    {
        std::cout << "This is an archer." << std::endl;
    }

protected:
    void attack() override
    {
        std::cout << "Archer shoots an arrow." << std::endl;
    }
};

class Knight : public Unit
{
public:
    void info() override
    {
        std::cout << "This is a knight." << std::endl;
    }

protected:
    void attack() override
    {
        std::cout << "Knight charges with a lance." << std::endl;
    }
};

class Building : public GameObject
{
public:
    void performAction() override
    {
        construct();
        defend();
    }

protected:
    virtual void construct() = 0;
    virtual void defend()
    {
        std::cout << "Building is being defended." << std::endl;
    }
};

class Barrack : public Building
{
public:
    void info() override
    {
        std::cout << "This is a barrack." << std::endl;
    }

protected:
    void construct() override
    {
        std::cout << "Barrack is under construction, preparing to train units." << std::endl;
    }
};

class Castle : public Building
{
public:
    void info() override
    {
        std::cout << "This is a castle." << std::endl;
    }

protected:
    void construct() override
    {
        std::cout << "Castle is under construction, fortifying defenses." << std::endl;
    }
};

class GameObjectFactory
{
public:
    virtual ~GameObjectFactory() = default;

    virtual Unit* createUnit() = 0;
    virtual Building* createBuilding() = 0;
};

class ArcherFactory : public GameObjectFactory
{
public:
    Unit* createUnit() override
    {
        return new Archer();
    }

    Building* createBuilding() override
    {
        return new Barrack();
    }
};

class KnightFactory : public GameObjectFactory
{
public:
    Unit* createUnit() override
    {
        return new Knight();
    }

    Building* createBuilding() override
    {
        return new Castle();
    }
};

class CompositeGameObject : public GameObject
{
public:
    void add(GameObject* child)
    {
        m_children.push_back(child);
    }

    void info() override
    {
        std::cout << "This is a composite object containing: " << m_children.size() << " objects." << std::endl;
        for (auto child : m_children)
        {
            child->info();
        }
    }

    void performAction() override
    {
        for (auto child : m_children)
        {
            child->performAction();
        }
    }

    ~CompositeGameObject() override
    {
        for (auto child : m_children)
        {
            delete child;
        }
        m_children.clear();
    }

private:
    std::vector<GameObject*> m_children;
};

void testGameObjects()
{
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    GameObjectFactory* archerFactory = new ArcherFactory();
    GameObject* archer = archerFactory->createUnit();
    GameObject* barrack = archerFactory->createBuilding();
    archer->info();
    assert(buffer.str() == "This is an archer.\n");

    buffer.str("");
    buffer.clear();

    barrack->info();
    assert(buffer.str() == "This is a barrack.\n");

    std::cout.rdbuf(oldCout);

    delete archerFactory;
    delete archer;
    delete barrack;
}

void testUnitActions()
{
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    GameObjectFactory* knFactory = new KnightFactory();
    GameObject* knight = knFactory->createUnit();
    GameObject* castle = knFactory->createBuilding();
    knight->performAction();
    assert(buffer.str() == "Knight charges with a lance.\nUnit moves.\nUnit defends itself.\n");

    buffer.str("");
    buffer.clear();

    castle->performAction();
    assert(buffer.str() == "Castle is under construction, fortifying defenses.\nBuilding is being defended.\n");

    std::cout.rdbuf(oldCout);

    delete knFactory;
    delete knight;
    delete castle;
}

void testComposite()
{
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    GameObjectFactory* archerFactory = new ArcherFactory();
    GameObjectFactory* knightFactory = new KnightFactory();
    GameObject* archer = archerFactory->createUnit();
    GameObject* barrack = archerFactory->createBuilding();
    GameObject* knight = knightFactory->createUnit();
    GameObject* castle = knightFactory->createBuilding();

    CompositeGameObject* composite = new CompositeGameObject();
    composite->add(archer);
    composite->add(barrack);
    composite->add(knight);
    composite->add(castle);
    composite->info();
    assert(buffer.str() == "This is a composite object containing: 4 objects.\nThis is an archer.\nThis is a barrack.\nThis is a knight.\nThis is a castle.\n");

    buffer.str("");
    buffer.clear();

    composite->performAction();
    assert(buffer.str() == "Archer shoots an arrow.\nUnit moves.\nUnit defends itself.\nBarrack is under construction, preparing to train units.\nBuilding is being defended.\nKnight charges with a lance.\nUnit moves.\nUnit defends itself.\nCastle is under construction, fortifying defenses.\nBuilding is being defended.\n");

    std::cout.rdbuf(oldCout);

    delete archerFactory;
    delete knightFactory;
    delete composite;
}

// Used patterns - Abstract Factory (instead of Builder), Composite, Template Method
int main() {
    testGameObjects();
    testUnitActions();
    testComposite();
}