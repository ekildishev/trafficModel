#pragma once

#include <QMainWindow>
#include <QDebug>
#include <QLabel>

#include "TrafficLightSystem.h"
#include "TrafficPath.h"
#include "PedSystem.h"
#include "Car.h"


namespace Ui
{
class MainWindow;
}

class MainWindow: public QMainWindow
{
Q_OBJECT

public:
    static bool debugMode;

    MainWindow();

    /**
     * @brief Проверяет наличие машин в коллизиях
     * @param circles Упорядоченное множество коллизий
     * @param except Машина, мнение которой не учитывается
     * @return Номер коллизии, в которой нашлась машина
     */
    int isCarsInCollision(const QVector<Car::Circle> &circles, Car *except);

    void removeCar(Car *car);

    const QVector<TrafficPath *> &getTrafficPaths() const;

    void updateImage();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void initBackground();

    void initGraph();

    void initPeds();

    void initLights();

    QList<Car *> cars;                  // destroy
    QList<TrafficLight *> lights;
    QVector<PedSystem *> pedSystems;    // destroy

    QLabel *background;                 // destroy

    QVector<TrafficPath *> trafficPaths{15, nullptr};
    QVector<TrafficLightSystem *> trafficLights{10, nullptr};

    Ui::MainWindow *ui = nullptr;
};