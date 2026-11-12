#include "food.hpp"

Food::Food() : cords_{0, 0}, generator_(std::chrono::steady_clock::now().time_since_epoch().count()) {}

void Food::spawn(int width, int height, const Snake &snake){
    std::uniform_int_distribution<> distX(0, width - 1);    // width diapason
    std::uniform_int_distribution<> distY(0, height - 1);   // height diapason
    
    Point point{};
    do {
        point.x = distX(generator_); 
        point.y = distY(generator_);
    } while (snake.contains(point));   

    cords_ = point;
}

const Point Food::getCords() const{
    return cords_;
}