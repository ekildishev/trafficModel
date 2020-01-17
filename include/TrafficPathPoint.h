#pragma once

#include <QPointF>

#include "TrafficLight.h"

class TrafficPathPoint: public QPointF
{
    bool isTemporary_ = false;

    TrafficLight *light = nullptr;

    struct
    {
        TrafficPathPoint *left = nullptr;
        TrafficPathPoint *right = nullptr;
    } sidePoint;

    struct {
        int vId = -1;
        int gId = -1;
    } id;

    qreal velocityMax = 999999;
    qreal velocity = 0; // начальная скорость

    int spawnRadius = 10;

    bool isBusStop_ = false;

public:
    using QPointF::QPointF;

    TrafficPathPoint(qreal xpos, qreal ypos);

    TrafficPathPoint(const TrafficPathPoint &) = default;
    TrafficPathPoint(TrafficPathPoint &&) = default;

    TrafficPathPoint &operator=(const TrafficPathPoint &) = default;
    TrafficPathPoint &operator=(TrafficPathPoint &&) = default;

    bool isTemporary() const;

    void setIsTemporary(bool isTemporary);

    qreal getVelocityMax() const;

    void setVelocityMax(qreal velocityMax);

    auto getSidePoints() const -> decltype(sidePoint);

    void setSidePoints(TrafficPathPoint *left, TrafficPathPoint *right);

    void setLight(TrafficLight *light);

    qreal getVelocity() const;

    void setVelocity(qreal velocity);

    int getSpawnRadius() const;

    void setSpawnRadius(int spawnRadius);

    void setId(int vId, int gId);

    auto getId() const -> decltype(id);

    bool isStop() const;

    bool isBusStop() const;

    void setIsBusStop(bool isBusStop);
};