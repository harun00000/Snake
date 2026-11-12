#pragma once

#include "snake.hpp"
#include <random>
#include <chrono>

class Food{
private:
    Point cords_;
    std::mt19937 generator_;

public:
    Food();

    void spawn(int width, int height, const Snake &snake);
    const Point getCords() const;
};


