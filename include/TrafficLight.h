#pragma once

#include <QLabel>

class MainWindow;

class TrafficLight: public QLabel
{
Q_OBJECT

public:
    explicit TrafficLight(int rotation, bool isPed = false, MainWindow *parent = nullptr);

    void setActiveLight(int light);

    int getActiveLight();

    void updateImage();

private:
    QPixmap getActiveLightPixmap(int light);

    static QPixmap *activeLightPixmap;
    static QPixmap *backgroundPixmap;
    static QPixmap *activeLightPedPixmap;
    static QPixmap *backgroundPedPixmap;

    MainWindow *parent;

    int activeLight = 0;
    int rotation;
    bool isPed;
};