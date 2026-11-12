#include "game.hpp"
#include "snake.hpp"

Game::Game(int width, int height) : width_(width), height_(height), snake_(width, height), isRunning_(true), score_(0), gameOver_(false) {
    food_.spawn(width_, height_, snake_);   // cause we need food at start
}

void Game::Update(){
    snake_.move();

    const Point curHead = snake_.getHead();
    const Point foodPos = food_.getCords();

    if (curHead.x == foodPos.x && curHead.y == foodPos.y) 
    {
        snake_.grow();
        food_.spawn(width_, height_, snake_);
        score_++;
    }

    // check borders
    if (curHead.x >= width_ || curHead.x < 0 || 
        curHead.y >= height_ || curHead.y <0 ||
        snake_.checkSelfCollision() == true)
    {
        isRunning_ = false;
    } 

    if (isRunning_ == false) gameOver_ = true;
}

bool Game::isRunning() const{
    return isRunning_;
}

Snake &Game::getSnake(){
    return snake_;
}

const Snake &Game::getSnake() const{
    return snake_;
}

const Food& Game::getFood() const{
    return food_;
}

int Game::getScore() const{
    return score_;
}

bool Game::gameOver() const{
    return gameOver_;
}