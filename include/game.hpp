#pragma once

#include "food.hpp"
#include "snake.hpp"

class Game{
private:
    int height_;
    int width_;

    Snake snake_;
    Food food_;

    bool isRunning_;
    int score_;
    bool gameOver_;

public:
    Game(int width, int height);

    void Update();

    bool isRunning() const;
    bool gameOver() const;
    Snake &getSnake();              // for changing
    const Snake &getSnake() const;  // for reading
    const Food &getFood() const;
    int getScore() const;

};