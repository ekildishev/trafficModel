#pragma once

#include <QLabel>
#include "TrafficLightSystem.h"

class MainWindow;

class PedSystem
{
    MainWindow *parent = nullptr;
    QPixmap *pedPixmap = new QPixmap{":/image/Ped.png"};
    QLabel *label;

    bool isHorizontal;
    TrafficLightSystem *lightSystem = nullptr;

    struct PedData {
        QPointF position;
        QPointF delta;
    };

protected:
    size_t maxPeds = 5;
    QList<PedData> peds{};


    bool isActive = false;
    int activeTime = 0;

    PedData spawnPed();

    bool isPedOnBorder(const PedData &ped);

    void updatePedData(int msec);

public:
    PedSystem(bool isHorizontal, QRect geometry, TrafficLightSystem *lightSystem, MainWindow *parent);

    virtual void updateData(int msec);

    virtual void updateImage();

    void activate(int time);

    QLabel *getLabel();

    ~PedSystem();
};