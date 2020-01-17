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

class MainWindow: public QMainWindow, public UpdatableImageInterface
{
Q_OBJECT

private:
    void initBackground();

    void initGraph();

    void initPeds();

    void initLights();

    QList<Car *> cars;
    QVector<PedSystem *> pedSystems;

    QLabel *background;

    QVector<TrafficPath *> trafficPaths{16, nullptr};
    QVector<TrafficLightSystem *> trafficLights{10, nullptr};

    QList<UpdatableImageInterface *> updateImageList{this};
    QList<UpdatableDataInterface *> updateDataList{};

    Ui::MainWindow *ui = nullptr;

protected:
    void keyPressEvent(QKeyEvent *event) override;

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

    void updateImage() override;

    QLabel *drawCircle(const Car::Circle &circle, const QColor &color = {255, 171, 0, 150});

    QLabel *drawSquare(const Car::Circle &circle, const QColor &color = {255, 171, 0, 150});

    ~MainWindow() override;
};