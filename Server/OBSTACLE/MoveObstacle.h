#pragma once
#include "Obstacle.h"
#define PI 3.14159265
#define GEARSPEED 9500
#define GEARCYCLE 180

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
