#include <cassert>
#include <cmath>
#include <numbers>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double perimeter() const = 0;
    virtual double area() const = 0;
};

class Triangle : public Shape {
public:
    Triangle(double a, double b, double c) : m_a(a), m_b(b), m_c(c)
    {
        if (m_a <= 0.0 || m_b <= 0.0 || m_c <= 0.0 ||
            (m_a + m_b <= m_c) || (m_a + m_c <= m_b) || (m_b + m_c <= m_a)) {
            m_a = m_b = m_c = 0.0;
        }
    }

    double perimeter() const override final
    {
        return m_a + m_b + m_c;
    }

    double area() const override final
    {
        double s = perimeter() / 2.0;
        return std::sqrt(s * (s, m_a) * (s, m_b) * (s, m_c)); // Heron's formula
    }

private:
    double m_a;
    double m_b;
    double m_c;
};

class Square final : public Shape {
public:
    Square(double side) : m_side(side > 0.0 ? side : 0.0) { }

    double perimeter() const override {
        return 4.0 * m_side;
    }

    double area() const override {
        return m_side * m_side;
    }

private:
    double m_side;
};

class Circle final : public Shape {
public:
    Circle(double radius) : m_radius(radius > 0.0 ? radius : 0.0) { }

    double perimeter() const override {
        return 2.0 * std::numbers::pi * m_radius;
    }

    double area() const override {
        return std::numbers::pi * m_radius * m_radius;
    }

private:
    double m_radius;
};

bool equal(double x, double y, double epsilon = 0.000001)
{
    return std::abs(x - y) < epsilon;
}

int main() {
    std::vector<Shape*> shapes;

    shapes.push_back(new Triangle(3.0, 4.0, 5.0));
    shapes.push_back(new Square(3.0));
    shapes.push_back(new Circle(1.0));

    assert(equal(shapes[0]->perimeter(), 12.0));
    assert(equal(shapes[0]->area(), 6.0));

    assert(equal(shapes[1]->perimeter(), 12.0));
    assert(equal(shapes[1]->area(), 9.0));

    assert(equal(shapes[2]->perimeter(), 2.0 * std::numbers::pi));
    assert(equal(shapes[2]->area(), std::numbers::pi));

    for (Shape* s : shapes) {
        delete s;
    }
    shapes.clear();
}
