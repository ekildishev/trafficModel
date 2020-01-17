#pragma once

#include <QLabel>
#include "Interface.h"

class MainWindow;

class TrafficLight: public QLabel, public UpdatableImageInterface
{
Q_OBJECT

private:
    QPixmap activeLightPixmap;
    QPixmap backgroundPixmap;
    QPixmap activeLightPedPixmap;
    QPixmap backgroundPedPixmap;

    MainWindow *parent;

    int activeLight = 0;
    int rotation;
    bool isPed;

    QPixmap getActiveLightPixmap(int light);

public:
    explicit TrafficLight(int rotation, bool isPed = false, MainWindow *parent = nullptr);

    void setActiveLight(int light);

    int getActiveLight();

    void updateImage() override;
};