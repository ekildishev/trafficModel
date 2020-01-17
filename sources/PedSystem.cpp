#include <random>
#include <QWidget>
#include <QPainter>

#include "MainWindow.h"
#include "PedSystem.h"

PedSystem::PedSystem(bool isHorizontal, QRect geometry, TrafficLightSystem *lightSystem, MainWindow *parent)
    : isHorizontal(isHorizontal),
      lightSystem(lightSystem),
      parent(parent),
      label(new QLabel{dynamic_cast<QWidget *>(parent)})
{
    label->setGeometry(geometry);
    label->show();
}

PedSystem::~PedSystem()
{
    delete pedPixmap;
    delete label;
}

void PedSystem::activate(int time)
{
    isActive = true;
    activeTime = time;
}

void PedSystem::updateData(int msec)
{
    if (lightSystem->isPedTime() && !isActive) {
        activate(lightSystem->getPedTime());
    }
    if (!lightSystem->isPedTime()) {
        isActive = false;
        peds.clear();
    }

    updatePedData(msec);
}

PedSystem::PedData PedSystem::spawnPed()
{
    const static qreal PED_VELOCITY = 25;
    std::mt19937 generator{std::random_device{}()};

    std::uniform_int_distribution<size_t> randomSide{0, 1};
    std::uniform_real_distribution<qreal> randomVelocity{PED_VELOCITY, PED_VELOCITY + 15};

    qreal border = pedPixmap->width();
    std::uniform_real_distribution<qreal> randomOffset{
        border,
        (isHorizontal ? label->geometry().height() : label->geometry().width()) - border
    };

    if (isHorizontal) {
        bool side = randomSide(generator);
        return {
            {
                side ? label->geometry().width() - 0.1 : 0.1,
                randomOffset(generator),
            },
            {
                randomVelocity(generator) * (side ? -1 : 1),
                0,
            }
        };
    }

    bool side = randomSide(generator);
    return {
        {
            randomOffset(generator),
            side ? label->geometry().height() - 0.1 : 0.1,
        },
        {
            0,
            randomVelocity(generator) * (side ? -1 : 1),
        }
    };

}

void PedSystem::updatePedData(int msec)
{
    static const int TIME_LIMIT = 3750;

    if (!isActive) {
        return;
    }

    if (peds.size() < maxPeds && activeTime >= TIME_LIMIT) {
        peds.push_back(spawnPed());
    }

    for (auto it = peds.begin(); it != peds.end();) {
        if (isPedOnBorder(*it)) {
            it = peds.erase(it);
            continue;
        }

        it->position += it->delta * msec / 1000;
        ++it;
    }

    activeTime -= msec;
    if (activeTime <= 0) {
        peds.clear();
        isActive = false;
    }
}

bool PedSystem::isPedOnBorder(const PedSystem::PedData &ped)
{
    if (isHorizontal) {
        return ped.position.x() <= 0 || ped.position.x() >= label->geometry().width();
    }

    return ped.position.y() <= 0 || ped.position.y() >= label->geometry().height();
}

void PedSystem::updateImage()
{
    const QRect &geometry = label->geometry();

    QPixmap newBackgroundPixmap(geometry.width(), geometry.height());
    newBackgroundPixmap.fill(Qt::transparent);

    QPainter painter{&newBackgroundPixmap};

    for (const PedData &data : peds) {
        painter.drawPixmap(
            data.position.x() - pedPixmap->width() / 2,
            data.position.y() - pedPixmap->height() / 2,
            *pedPixmap
        );
    }

    painter.end();

    // обновляем label
    label->setPixmap(newBackgroundPixmap);
}

QLabel *PedSystem::getLabel()
{
    return label;
}
