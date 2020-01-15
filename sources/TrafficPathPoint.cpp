#include "TrafficPathPoint.h"
TrafficPathPoint::TrafficPathPoint(qreal xpos, qreal ypos)
    : QPointF(xpos, ypos)
{}

bool TrafficPathPoint::isTemporary() const
{
    return isTemporary_;
}

void TrafficPathPoint::setIsTemporary(bool isTemporary)
{
    TrafficPathPoint::isTemporary_ = isTemporary;
}

qreal TrafficPathPoint::getVelocityMax() const
{
    return velocityMax;
}

void TrafficPathPoint::setVelocityMax(qreal velocityMax_)
{
    TrafficPathPoint::velocityMax = velocityMax_;
}

bool TrafficPathPoint::isStop() const
{
    if (!light) {
        return false;
    }

    return light->getActiveLight() != 2;

}

void TrafficPathPoint::setLight(TrafficLight *newLight)
{
    TrafficPathPoint::light = newLight;
}

auto TrafficPathPoint::getSidePoints() const -> decltype(sidePoint)
{
    return sidePoint;
}

void TrafficPathPoint::setSidePoints(TrafficPathPoint *left, TrafficPathPoint *right)
{
    sidePoint = {
        left,
        right
    };
}

void TrafficPathPoint::setId(int vId, int gId)
{
    id = {
        vId,
        gId,
    };
}

auto TrafficPathPoint::getId() const -> decltype(id)
{
    return id;
}

qreal TrafficPathPoint::getVelocity() const
{
    return velocity;
}

void TrafficPathPoint::setVelocity(qreal newVelocity)
{
    TrafficPathPoint::velocity = newVelocity;
}

int TrafficPathPoint::getSpawnRadius() const
{
    return spawnRadius;
}

void TrafficPathPoint::setSpawnRadius(int newSpawnRadius)
{
    TrafficPathPoint::spawnRadius = newSpawnRadius;
}

bool TrafficPathPoint::isBusStop() const
{
    return isBusStop_;
}

void TrafficPathPoint::setIsBusStop(bool isBusStop)
{
    isBusStop_ = isBusStop;
}
