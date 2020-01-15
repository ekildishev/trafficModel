#pragma once

#include <QObject>
#include "TrafficLight.h"

class TrafficLightSystem: public QObject
{
Q_OBJECT

public:
    explicit TrafficLightSystem() = default;

    void updateData(int msec);

    void addLight(TrafficLight *light, int index);

    void addPedLight(TrafficLight *light);

    const QVector<TrafficLight *> &getLights() const;

    void setResetTime(int odd, int even, int ped);

    bool isPedTime() const;

    int getPedTime();

    // TODO: destructor

private:
    int timeToNextResetOdd = 6000;
    int timeToNextReset = 6000;
    int timePed = 6000;

    int timeMiddleState = 1000;
    int timeToNext = 0;

    bool nextStateFullRed = false;

    QVector<TrafficLight *> lights{10, nullptr};
    QVector<TrafficLight *> pedLights{};

};