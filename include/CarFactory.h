#pragma once

#include <QVector>
#include <random>

#include "Car.h"

class MainWindow;

class CarFactory
{
    struct CarData {
        QString pixmapPath{};
        QRect size{};
        QPair<qreal, qreal> randomVelocityRange{};
    };

public:
    using CarDataContainer = QVector<CarData>;

    static const CarDataContainer carData;

    static Car *createCar(MainWindow *parent, TrafficPath *path);
};