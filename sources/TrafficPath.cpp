#include "TrafficPath.h"

const QVector<TrafficPathPoint *> &TrafficPath::getPoints() const
{
    return points;
}

void TrafficPath::addPoint(TrafficPathPoint *point, int index)
{
    points[index] = point;
}

TrafficPath::~TrafficPath()
{
    for (auto *point : points) {
        delete point;
    }
}
