#include "snake.hpp"

Snake::Snake(int width, int height) : dx_(1), dy_(0), canGrow_(false) {
    int halfWidth = width / 2;
    int halfHeight = height / 2;

    body.push_back({halfWidth, halfHeight});
    body.push_back({halfWidth - 1, halfHeight});
}

void Snake::grow(){
    canGrow_ = true;
}

void Snake::setNextMove(int dx, int dy){
    // ban potencial reserve turns
    if (dx == -dx_ && dy == -dy_) return;

    dx_ = dx;
    dy_ = dy;
}

void Snake::move(){
    Point curHead = body.front();
    
    // set newHead
    Point newHead{};
    newHead.x = curHead.x + dx_;
    newHead.y = curHead.y + dy_;
    body.push_front(newHead);

    // del tail if not grow
    if (!canGrow_)
    {
        body.pop_back();
    } else {
        canGrow_ = false;
    }
}

size_t Snake::size() const{
    return body.size();
}

const std::deque<Point>& Snake::getBody() const{
    return body;
}

const Point Snake::getHead() const {
    return body.front();
}

bool Snake::checkSelfCollision() const{
    Point curHead = body.front();

    // check every point in snake
    for (size_t idx = 1; idx < body.size(); ++idx)
    {
        if (body[idx].x == curHead.x && body[idx].y == curHead.y)
            return true;
    }
    return false;
}

bool Snake::contains(const Point& curPoint) const{
    for (size_t idx = 0; idx < body.size(); ++idx)
    {
        if (body[idx].x == curPoint.x && body[idx].y == curPoint.y)
            return true;
    }
    return false;
}

