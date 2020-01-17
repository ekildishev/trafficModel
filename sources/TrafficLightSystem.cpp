#include <QTimer>
#include "TrafficLightSystem.h"

void TrafficLightSystem::addLight(TrafficLight *light, int index)
{
    lights[index] = light;
}

void TrafficLightSystem::updateData(int msec)
{
    if (timeToNext <= 0) {
        auto *timer = new QTimer{};         // для смены с желтого

        int newStateOdd = 0;
        for (size_t i = 0; i < lights.size(); i += 2) {
            if (!lights[i]) {
                break;
            }

            TrafficLight *light = lights[i];
            if (i == 0) {
                newStateOdd = nextStateFullRed ? 0 : ((light->getActiveLight() + 2) % 4);
            }

            if (newStateOdd != light->getActiveLight()) {
                light->setActiveLight(newStateOdd == 2 ? 3 : 1);       // ставим желтый
            }

            connect(timer, &QTimer::timeout, [light, newStateOdd]() {
                light->setActiveLight(newStateOdd);
            });
        }

        int newStateEven = nextStateFullRed ? 0 : ((newStateOdd + 2) % 4);
        for (size_t i = 1; i < lights.size(); i += 2) {
            if (!lights[i]) {
                break;
            }
            TrafficLight *light = lights[i];

            if (newStateEven != light->getActiveLight()) {
                light->setActiveLight(newStateEven == 2 ? 3 : 1);       // ставим желтый
            }

            connect(timer, &QTimer::timeout, [light, newStateEven]() {
                light->setActiveLight(newStateEven);
            });
        }

        // Если все светофоры станут красными - нужно включить пешеходные переходы
        bool fullRedCache = nextStateFullRed;           // сохраним текущее
        connect(timer, &QTimer::timeout, [this, fullRedCache]() {
            for (TrafficLight *light: pedLights) {
                light->setActiveLight(fullRedCache);
            }
        });

        connect(timer, &QTimer::timeout, [timer]() {
            timer->stop();
            timer->deleteLater();
        });
        timer->setInterval(timeMiddleState);
        timer->start();

        if (fullRedCache) {
            timeToNext = timePed;
        } else {
            timeToNext = newStateOdd == 0 ? timeToNextReset : timeToNextResetOdd;
        }
        nextStateFullRed = newStateOdd == 0 && !nextStateFullRed;
    }

    timeToNext -= msec;
}

const QVector<TrafficLight *> &TrafficLightSystem::getLights() const
{
    return lights;
}

void TrafficLightSystem::addPedLight(TrafficLight *light)
{
    pedLights.push_back(light);
}

void TrafficLightSystem::setResetTime(int odd, int even, int ped)
{
    timeToNextResetOdd = odd;
    timeToNextReset = even;
    timePed = ped;
}

bool TrafficLightSystem::isPedTime() const
{
    if (pedLights.empty()) {
        return false;
    }

    return pedLights.front()->getActiveLight();
}

int TrafficLightSystem::getPedTime()
{
    return timePed;
}

TrafficLightSystem::~TrafficLightSystem()
{
    for (auto *light : lights) {
        delete light;
    }
    for (auto *light : pedLights) {
        delete light;
    }
}
