#include <QIcon>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>
#include <QDebug>
#include <QHash>

#include "TrafficLight.h"
#include "MainWindow.h"

TrafficLight::TrafficLight(int rotation, bool isPed, MainWindow *parent)
    : QLabel(dynamic_cast<QWidget *>(parent)),
      rotation(rotation),
      isPed(isPed),
      parent(parent)
{
    backgroundPixmap = QPixmap{":image/TrafficLight.png"};
    activeLightPixmap = QPixmap{":image/TrafficLightActive.png"};
    backgroundPedPixmap = QPixmap{":image/TrafficLightPed.png"};
    activeLightPedPixmap = QPixmap{":image/TrafficLightPedActive.png"};

    this->updateImage();
}

void TrafficLight::setActiveLight(int light)
{
    // model
    if (!(light == 3 || (light >= 0 && light <= 2 - isPed))) {
        return;
    }

    activeLight = light;
}

QPixmap TrafficLight::getActiveLightPixmap(int light)
{
    static const QHash<bool, QHash<int, QColor>> COLORS = {
        {
            0,
            {
                {0, QColor{255, 0, 0, 255}},
                {1, QColor{255, 255, 0, 255}},
                {2, QColor{0, 255, 0, 255}},
                {3, QColor{255, 255, 0, 255}},
            }
        },
        {
            1,
            {
                {0, QColor{255, 0, 0, 255}},
                {1, QColor{0, 255, 0, 255}},
            }
        },
    };

    auto currentActiveLight = isPed ? activeLightPedPixmap : activeLightPixmap;

    // рисуем лампочку светофора
    const QPixmap& activePixmapLayer{currentActiveLight};
    QPixmap activePixmap{currentActiveLight.size()};

    activePixmap.fill(COLORS[isPed][light]);
    activePixmap.setMask(
        activePixmapLayer.createMaskFromColor(Qt::transparent)
    );

    return activePixmap;
}

void TrafficLight::updateImage()
{

    static const QHash<bool, QHash<int, int>> DY = {
        {
            0,
            {
                {0, 0},
                {1, 9},
                {2, 18},
                {3, 9},
            },
        },
        {
            1,
            {
                {0, 0},
                {1, 12},
            },
        },
    };

    if (!(activeLight == 3 || (activeLight >= 0 && activeLight <= 2 - isPed))) {
        return;
    }

    // запомним, с какими картинками работаем
    auto currentBackground = isPed ? backgroundPedPixmap : backgroundPixmap;


    // мини-костыль, реализующий поворот картинки
    QList<int> sizes{
        currentBackground.size().width(),
        currentBackground.size().height(),
    };
    int maxSize = *std::max_element(sizes.cbegin(), sizes.cend());

    QPixmap newBackgroundPixmap(maxSize, maxSize);
    newBackgroundPixmap.fill(Qt::transparent);

    QPainter painter{&newBackgroundPixmap};

    painter.rotate(rotation);
    if (rotation == 90) {
        painter.translate(maxSize / 4, -maxSize);
    } else if (rotation == 180) {
        painter.translate(-maxSize / 2, -maxSize);
    } else if (rotation == 270) {
        painter.translate(-maxSize * 3 / 4, 0);
    }

    // рисуем фон светофора и лампу
    painter.drawPixmap(0, 0, currentBackground);
    painter.drawPixmap(
        0,
        DY[isPed][activeLight],    // дваигаем вниз
        getActiveLightPixmap(activeLight)
    );
    if (activeLight == 3) {  // красный + желтый
        painter.drawPixmap(
            0,
            DY[isPed][0],    // дваигаем вниз
            getActiveLightPixmap(0)
        );
    }

    painter.end();

    // обновляем label
    this->setPixmap(newBackgroundPixmap);
}

int TrafficLight::getActiveLight()
{
    return activeLight;
}
