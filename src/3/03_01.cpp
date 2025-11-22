#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

struct Rectangle
{
    int x1, y1; // top left point
    int x2, y2; // bottom right point

    Rectangle() : x1(0), y1(0), x2(0), y2(0) { }

    Rectangle(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(std::max(x1, x2)), y2(std::max(y1, y2)) { }

    int area() const
    {
        return (x2 - x1) * (y2 - y1);
    }
};

Rectangle intersect(Rectangle const& r1, Rectangle const& r2)
{
    int x1 = std::max(r1.x1, r2.x1);
    int y1 = std::max(r1.y1, r2.y1);

    int x2 = std::min(r1.x2, r2.x2);
    int y2 = std::min(r1.y2, r2.y2);

    return Rectangle {x1, y1, x2, y2};
}

int intersection_area(std::vector<Rectangle> const& rectangles)
{
    if (rectangles.empty())
    {
        return 0;
    }

    Rectangle intersection = rectangles[0];
    for (size_t i = 1; i < rectangles.size(); ++i)
    {
        if (intersection.area() == 0)
        {
            return 0;
        }
        intersection = intersect(intersection, rectangles[i]);
    }

    return intersection.area();
}

Rectangle minimum_bounding_rectangle(const std::vector<Rectangle>& rectangles) {
    if (rectangles.empty())
    {
        return Rectangle {};
    }

    int min_x = rectangles[0].x1;
    int min_y = rectangles[0].y1;

    int max_x = rectangles[0].x2;
    int max_y = rectangles[0].y2;

    for (Rectangle const& rect : rectangles) {
        min_x = std::min(min_x, rect.x1);
        min_y = std::min(min_y, rect.y1);
        max_x = std::max(max_x, rect.x2);
        max_y = std::max(max_y, rect.y2);
    }

    return Rectangle {min_x, min_y, max_x, max_y};
}

int main() {
    // One rectangle
    {
        std::vector rects = {Rectangle {0, 0, 2, 1}};
        assert(intersection_area(rects) == 2);
        Rectangle r = minimum_bounding_rectangle(rects);
        assert(r.x1 == 0 && r.y1 == 0 && r.x2 == 2 && r.y2 == 1);
    }

    // Two intersecting rectangles
    {
        std::vector rects = {Rectangle {0, 0, 2, 1}, Rectangle {1, 0, 2, 2}};
        assert(intersection_area(rects) == 1);
        Rectangle r = minimum_bounding_rectangle(rects);
        assert(r.x1 == 0 && r.y1 == 0 && r.x2 == 2 && r.y2 == 2);
    }

    // Two non-intersecting rectangles
    {
        std::vector rects = {Rectangle {0, 0, 2, 1}, Rectangle {3, 3, 4, 4}};
        assert(intersection_area(rects) == 0);
        Rectangle r = minimum_bounding_rectangle(rects);
        assert(r.x1 == 0 && r.y1 == 0 && r.x2 == 4 && r.y2 == 4);
    }

    // Two rectangles (with one of them being degenerate/a line)
    {
        std::vector rects = { Rectangle {0, 0, 2, 1}, Rectangle {2, 0, 2, 3}};
        assert(intersection_area(rects) == 0);
        Rectangle r = minimum_bounding_rectangle(rects);
        assert(r.x1 == 0 && r.y1 == 0 && r.x2 == 2 && r.y2 == 3);
    }

    // Тhree intersecting rectangles
    {
        std::vector rects = { Rectangle {0, 0, 5, 5}, Rectangle {1, 1, 4, 2}, Rectangle {3, 1, 4, 5}};
        assert(intersection_area(rects) == 1);
        Rectangle r = minimum_bounding_rectangle(rects);
        assert(r.x1 == 0 && r.y1 == 0 && r.x2 == 5 && r.y2 == 5);
    }
}