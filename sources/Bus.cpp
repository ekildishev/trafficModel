#include <cmath>
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
        pedSystem->getLabel()->show();
        pedSystem->activate(stopTimeReset);

        // TODO: logic

        stopTime -= msec;

        if (stopTime < 0) {
            isStop = false;
            pedSystem->getLabel()->hide();
            pedSystem->activate(0);
        }
    }
}

void Bus::updateSpeed(int msec)
{
    if (isStop) {
        velocity = 0;
        return;
    }

    Car::updateSpeed(msec);

    if (!position.end->isStop() or stopDisable) {
        return;
    }

    QPointF delta = *position.end - position.current;
    qreal distance = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());

    if (distance > 30) {
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

    // TODO: label geometry
}

void Bus::setNewIndex(int index)
{
    Car::setNewIndex(index);

    stopDisable = false;

}

void BusPedSystem::updateData(int msec)
{
    if (!isActive) {
        peds.clear();
    }

    updatePedData(msec);
}
