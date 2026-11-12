#pragma once

#include <cstddef>
#include <deque>
#include <string>

struct Point{
    int x;
    int y;
};

class Snake{
private:
    std::deque<Point> body;
    int dx_;
    int dy_;
    bool canGrow_;

public:
    Snake(int width, int height);

    void grow();
    void setNextMove(int dx, int dy);
    void move();

    size_t size() const;
    const std::deque<Point>& getBody() const;
    const Point getHead() const;

    bool checkSelfCollision() const;              // for snake doesn't crash into herself
    bool contains(const Point& curPoint) const;   // for food doesn't spawn in snake
};