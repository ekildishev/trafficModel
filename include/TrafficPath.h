#pragma once

#include <QObject>
#include "TrafficPathPoint.h"

class TrafficPath: public QObject
{
    Q_OBJECT
public:
    const QVector<TrafficPathPoint*> &getPoints() const
    {
        return points;
    }

    void addPoint(TrafficPathPoint *point, int index){
        points[index] = point;
    }


private:
    QVector<TrafficPathPoint*> points{50, nullptr};

};