#pragma once

#include "TrafficPathPoint.h"

class TrafficPath
{
    QVector<TrafficPathPoint *> points{50, nullptr};

public:
    const QVector<TrafficPathPoint *> &getPoints() const;

    void addPoint(TrafficPathPoint *point, int index);

    ~TrafficPath();
};