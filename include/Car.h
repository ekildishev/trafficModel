#pragma once

#include <QLabel>
#include "Interface.h"
#include "TrafficPath.h"

class MainWindow;

class Car: public QLabel, public UpdatableDataInterface, public UpdatableImageInterface
{
    QPixmap *picture;
    TrafficPath *path;
    int pointIndex;

    QVector<QLabel *> debugHelp;

public:
    struct Circle
    {
        QPointF point{0, 0};
        qreal radius{0};
    };

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    virtual QVector<Circle> getFrontCollisions() const;

    virtual QVector<Circle> getSideCollisions(bool left) const;

    virtual void updateSpeed(int msec);

    virtual void setNewIndex(int index);

    void updatePosition(TrafficPathPoint *start, TrafficPathPoint *end);

    void updateRotation();

    void clearDebugHelp();

    struct
    {
        TrafficPathPoint *start = nullptr;
        TrafficPathPoint *end = nullptr;

        TrafficPathPoint current;
        qreal distance = 999999;
    } position;
    QRect size;

    qreal rotation = 0;
    qreal rotationAcceleration = 1;
    qreal rotationMax = 0;

    qreal velocity = 0;
    qreal acceleration = 35;
    qreal decceleration = 35;
    qreal velocityMax = 50;

    MainWindow *parent;

    int isDoingTurn = -1;           // -1 нет, 0 вправо, 1 влево

public:
    Car(QPixmap *picture, QRect size, TrafficPath *path, int startIndex, MainWindow *parent = nullptr);

    void updateData(int msec) override;

    void updateImage() override;

    void setRotationAcceleration(qreal rotationAcceleration);

    void setVelocityMax(qreal m);

    virtual QVector<Circle> getCollisions() const;

    /**
     * @brief Основные точки соприкосновения (чтобы проверять коллизии)
     * @return
     */
    virtual QVector<QPointF> getPivots() const;

    // перестроиться
    virtual bool doTurn(bool left);

    ~Car() override;

};