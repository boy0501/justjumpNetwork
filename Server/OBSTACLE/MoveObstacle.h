#pragma once
#include "Obstacle.h"

class MoveObstacle : public Obstacle
{
public:
    enum class MoveType {
        Horizontal,
        Vertical,
    };
    MoveObstacle(MoveObstacle::MoveType movetype);
    virtual ~MoveObstacle();
    virtual void update(float deltatime);
    MoveType mMoveType;
    float degree;


};
