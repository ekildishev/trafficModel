#include <cmath>
#include <MainWindow.h>
#include <Car.h>
#include "Bus.h"

Bus::Bus(QPixmap *picture, const QRect &size, TrafficPath *path, int startIndex, MainWindow *parent)
    : Car(picture, size, path, startIndex, parent)
{
    setRotationAcceleration(0.3);
}

void Bus::updateData(int msec)
{
    Car::updateData(msec);

    if (isStop) {
        pedSystem->activate(stopTimeReset + 10000);

        stopTime -= msec;
        if (stopTime < 0) {
            isStop = false;
            pedSystem->activate(0);
        }
    }

    pedSystem->updateData(msec);
}

void Bus::updateSpeed(int msec)
{
    if (isStop) {
        velocity = 0;
        return;
    }

    Car::updateSpeed(msec);

    if (!position.end->isBusStop() || stopDisable) {
        return;
    }

    QPointF delta = *position.end - position.current;
    qreal distance = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());

    if (distance > 10 && distance < 30) {
        velocity -= decceleration;
        if (velocity < 15) {
            velocity = 15;
        }
    }
    if (distance <= 10) {
        velocity = 0;

        stopTime = stopTimeReset;
        isStop = true;
        stopDisable = true;
    }
}

void Bus::updateImage()
{
    Car::updateImage();

    if (MainWindow::debugMode) {
        pedSystem->getLabel()->setStyleSheet("border: 1px solid #39A939;");
    } else {
        pedSystem->getLabel()->setStyleSheet("");
    }

    static const qreal SIZE = 24;

    QRect geometry{
        static_cast<int>(position.current.x() + size.height() / 2. - SIZE * 0.1),
        static_cast<int>(position.current.y() - SIZE / 2),
        static_cast<int>(SIZE),
        static_cast<int>(SIZE),
    };

    pedSystem->getLabel()->setGeometry(geometry);
    pedSystem->updateImage();
}

void Bus::setNewIndex(int index)
{
    Car::setNewIndex(index);

    stopDisable = false;
}

Bus::~Bus()
{
    delete pedSystem;
}

void BusPedSystem::updateData(int msec)
{
    if (!isActive) {
        peds.clear();
    }

    updatePedData(msec);
}
