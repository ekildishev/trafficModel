#pragma once

#include "Car.h"
#include "PedSystem.h"

class BusPedSystem: public PedSystem
{
public:
    using PedSystem::PedSystem;

    void updateData(int msec) override;
};

class Bus: public Car
{
    int stopTimeReset = 3500;
    int stopTime = -1;
    bool isStop = false;
    bool stopDisable = false;

    BusPedSystem *pedSystem = new BusPedSystem{
        true,
        {0, 0, 0, 0},
        nullptr,
        parent
    };

protected:
    void updateSpeed(int msec) override;

public:
protected:
    void setNewIndex(int index) override;
public:
    Bus(QPixmap *picture, const QRect &size, TrafficPath *path, int startIndex, MainWindow *parent);

    void updateData(int msec) override;

    void updateImage() override;

};