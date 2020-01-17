#include <QPixmap>
#include <QPainter>
#include <QLabel>
#include <QDebug>
#include <cmath>
#include <Car.h>
#include <TrafficPathPoint.h>

#include "MainWindow.h"
#include "Car.h"

#define TO_RADIANS 3.14159268 / 180
#define TO_DEGREES 180 / 3.14159268

inline qreal normalizeAngle(qreal angle, bool canBeZero)
{
    while (angle < 0 && (canBeZero || angle == 0)) {
        angle += 360;
    }
    while (angle > 360 && (!canBeZero || angle == 360)) {
        angle -= 360;
    }

    return angle;
}

inline qreal getNearestDelta(qreal left, qreal right)
{
    QList<qreal> nearests = {left - right,
                             left - right + 360,
                             left - right - 360,
    };

    qreal nearest = nearests.back();
    qreal nearestAbs = std::abs(nearest);
    for (qreal angle: nearests) {
        if (std::abs(angle) < nearestAbs) {
            nearest = angle;
            nearestAbs = std::abs(angle);
        }
    }

    return nearest;
}

qreal getAngleBetweenPoints(const QPointF &start, const QPointF &end)
{
    // считаем угол поворота
    QPointF delta = end - start;

    qreal length = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    qreal cos = delta.x() / length;
    qreal sin = -delta.y() / length;

    return normalizeAngle(std::acos(cos) * TO_DEGREES * (sin < 0 ? -1 : 1), sin > 0);
}

Car::Car(QPixmap *picture, QRect size, TrafficPath *path, int startIndex, MainWindow *parent)
    : picture(picture),
      path(path),
      size(size),
      pointIndex(startIndex),
      parent(parent),
      QLabel(dynamic_cast<QWidget *>(parent))
{
    Car::setNewIndex(startIndex);
    position.current = *position.start;

    rotationMax = getAngleBetweenPoints(*position.start, *position.end);
    rotation = rotationMax;

    if (position.start->getVelocity()) {
        velocity = position.start->getVelocity();
    }
}

void Car::updatePosition(TrafficPathPoint *start, TrafficPathPoint *end)
{
    // Если это временные объекты - удаляем
    if (position.start && position.start->isTemporary()) {
        delete position.start;
    }
    if (position.end && position.end != start && position.end->isTemporary()) {
        delete position.end;
    }

    position.distance = 999999;
    position.start = start;
    position.end = end;
}

void Car::updateRotation()
{
    // считаем угол поворота
    QPointF delta = *position.end - *position.start;

    qreal length = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    qreal sin = -delta.y() / length;

    qreal newRotationMax = getAngleBetweenPoints(*position.start, *position.end);
    qreal oldRotationMax = normalizeAngle(rotationMax, sin > 0);
    rotationMax += getNearestDelta(newRotationMax, oldRotationMax);
}

void Car::setNewIndex(int index)
{
    auto startPosition = position.end ? position.end : path->getPoints()[index];
    updatePosition(startPosition, path->getPoints()[index + 1]);

    position.current = *position.start;
    pointIndex = index;

    if (velocity > position.start->getVelocityMax()) {
        velocity = position.start->getVelocityMax();        // гарантируем непревышение
    }

    isDoingTurn = -1;
    updateRotation();
}

void Car::updateSpeed(int msec)
{
    QPointF delta = *position.end - position.current;
    qreal newDistance = delta.manhattanLength();

    // Обновление скорости
    qreal allowedSpeed = position.end->getVelocityMax();
    if (position.end->isStop() && newDistance < 50) {
        allowedSpeed = 10;
    }

    // Проверка коллизий
    int collisionHit = parent->isCarsInCollision(getFrontCollisions(), this);
    int collisionSideHit = -1;
    if (isDoingTurn != -1) {
        collisionSideHit = parent->isCarsInCollision(getSideCollisions(isDoingTurn), this);
    }

    if (collisionHit == 1 || collisionHit == 2 || (collisionSideHit < 2 && collisionSideHit != -1)) {
        velocity = 0;
        return;
    }
    if (collisionHit != -1) {
        velocity -= decceleration * msec / 1000;

        if (velocity < 0) {
            velocity = 0;
        }

        return;
    }

    if (velocity < velocityMax && velocity < allowedSpeed) {
        // Ускоряемся, если нет коллизий и превышения
        velocity += acceleration * msec / 1000;
        return;
    }

    if (velocity > allowedSpeed) {
        velocity -= decceleration * msec / 1000;
    }
}

void Car::updateData(int msec)
{
    QPointF delta = *position.end - position.current;
    qreal newDistance = delta.manhattanLength();

    // Проверка на переход на следующую дугу
    if (newDistance > position.distance || newDistance < 1) {
        if (position.end->isStop()) {       // ждем светофор
            velocity = 0;
            return;
        }
        if (newDistance > position.distance) {           // проехали
            qDebug() << "Miss!";
        }

        if (!path->getPoints()[pointIndex + 2]) {       // путь закончен
            parent->removeCar(this);
            return;
        }

        setNewIndex(pointIndex + 1);
    } else {
        position.distance = newDistance;
    }

    // Рассчет новой позиции
    {
        delta = *position.end - *position.start;

        qreal length = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
        qreal cos = delta.x() / length;
        qreal sin = -delta.y() / length;

        position.current += {
            cos * velocity * msec / 1000,
            -sin * velocity * msec / 1000
        };
    }

    updateSpeed(msec);

    if (velocity > 0) {
        if (rotationMax < rotation) {
            rotation -= rotationAcceleration;
        } else if (rotationMax > rotation) {
            rotation += rotationAcceleration;
        }

        // поворот окончен
        if (rotationMax - rotationAcceleration <= rotation && rotation <= rotationMax + rotationAcceleration) {
            rotation = rotationMax;
        }
    }
}

void Car::updateImage()
{
    static const qreal MULTIPLIER = 1;

    QRect geometry{
        static_cast<int>(position.current.x()
            - picture->width() * MULTIPLIER / 2), // размер label в 2 раза больше, чем изображения
        static_cast<int>(position.current.y() - picture->height() * MULTIPLIER / 2),
        static_cast<int>(picture->width() * MULTIPLIER),
        static_cast<int>(picture->height() * MULTIPLIER)
    };

    this->setGeometry(geometry);

    QPixmap newBackgroundPixmap(geometry.width(), geometry.height());
    newBackgroundPixmap.fill(Qt::transparent);

    QPainter painter{&newBackgroundPixmap};

    painter.translate(+geometry.width() / 2, +geometry.height() / 2);
    painter.rotate(-rotation);

    painter.drawPixmap(
        -picture->width() / 2,
        -picture->height() / 2,
        *picture);

    painter.end();

    // обновляем label
    this->setPixmap(newBackgroundPixmap);


    if (MainWindow::debugMode) {
        this->setStyleSheet("border: 1px solid rgb(198, 255, 105);");
        clearDebugHelp();

        auto collisions = getCollisions();
        for (auto &collision: collisions) {
            QLabel *label = parent->drawCircle(collision);
            debugHelp.push_back(label);
        }

        auto pivots = getPivots();
        for (auto &pivot: pivots) {
            QLabel *label = parent->drawCircle({pivot, 3}, {255, 255, 255, 180});
            debugHelp.push_back(label);
        }

        for (const QPointF &point: {*position.start, *position.end}) {
            QLabel *label = parent->drawSquare(
                {
                    point,
                    8,
                },
                {255, 255, 255, 220}
            );
            label->show();
            debugHelp.push_back(label);
        }
    } else {
        if (!debugHelp.empty()) {
            clearDebugHelp();
        }
        this->setStyleSheet("");
    }
}

QVector<Car::Circle> Car::getCollisions() const
{
    auto result = getFrontCollisions();

    if (isDoingTurn != -1) {
        result.append(getSideCollisions(isDoingTurn));
    }
    return result;
}

QVector<Car::Circle> Car::getFrontCollisions() const
{
    qreal COLLISION_RADIUS = size.height() * 0.8;
    size_t COLLISION_AMOUNT = 3 + int(velocity / 30);

    qreal COLLISION_DELTA = COLLISION_RADIUS * (0.4 + 0.8 * velocity / 80);
    if (COLLISION_DELTA > COLLISION_RADIUS * 1.5) {
        COLLISION_DELTA = COLLISION_RADIUS * 0.8;
    }

    if (velocity < 10) {            // уменьшаем коллизии, если машина почти стоит
        COLLISION_RADIUS -= 4;
        COLLISION_AMOUNT -= 1;
    }
    if (velocity < 5) {            // уменьшаем коллизии, если машина почти стоит
        COLLISION_RADIUS -= 2;
        COLLISION_AMOUNT -= 1;
    }

    if (getNearestDelta(rotation, rotationMax) > rotationAcceleration) {
        // машина в процессе поворота
        COLLISION_AMOUNT += 0;
    }

    QVector<Circle> result{};
    for (size_t i = 0; i < COLLISION_AMOUNT; ++i) {
        qreal newRotation = rotation;
        qreal length = i * COLLISION_DELTA + size.width() * 0.5 + COLLISION_RADIUS * 1 / 3;

        qreal angleDelta = getNearestDelta(rotation, rotationMax);
        if (std::abs(angleDelta) > rotationAcceleration / 2) {
            // машина в процессе поворота
            newRotation += std::pow(qreal(i) + 1, 1.5) * ((rotationMax - rotation > 0) ? 4 : -4);
        }

        QPointF delta{
            length * std::cos(-newRotation * TO_RADIANS),
            length * std::sin(-newRotation * TO_RADIANS),
        };

        result.push_back({
                             position.current + delta,
                             COLLISION_RADIUS,
                         });
    }

    return result;
}

QVector<Car::Circle> Car::getSideCollisions(bool left) const
{
    qreal COLLISION_RADIUS = 8;
    size_t COLLISION_AMOUNT = 6;
    qreal COLLISION_DELTA = 6;
    qreal COLLISION_OFFSET = 20;

    QVector<Circle> result{};
    for (size_t i = 0; i < COLLISION_AMOUNT; ++i) {
        qreal newRotation = rotation;
        qreal length = -(i * COLLISION_DELTA - COLLISION_OFFSET);

        QPointF delta{
            length * std::cos(-newRotation * TO_RADIANS),
            length * std::sin(-newRotation * TO_RADIANS),
        };
        QPointF h{
            size.height() * 1 * std::cos(-(rotation + 90) * TO_RADIANS),
            size.height() * 1 * std::sin(-(rotation + 90) * TO_RADIANS),
        };

        result.push_back({
                             position.current + delta + (left ? +h : -h),
                             COLLISION_RADIUS,
                         });
    }

    return result;
}

QVector<QPointF> Car::getPivots() const
{
    qreal length = size.width() / 2.;

    QPointF delta{
        length * std::cos(-rotation * TO_RADIANS),
        length * std::sin(-rotation * TO_RADIANS),
    };
    QPointF h{
        size.height() / 2. * std::cos(-(rotation + 90) * TO_RADIANS),
        size.height() / 2. * std::sin(-(rotation + 90) * TO_RADIANS),
    };
    return {
        position.current + h,
        position.current - h,
        position.current + delta,
        position.current - delta,
        position.current + delta + h,
        position.current - delta + h,
        position.current + delta - h,
        position.current - delta - h,
    };

}

void Car::clearDebugHelp()
{
    for (QLabel *object: debugHelp) {
        object->deleteLater();
    }
    debugHelp.clear();
}

Car::~Car()
{
    delete picture;
    for (QLabel *label:debugHelp) {
        delete label;
    }

    if (position.start && position.start->isTemporary()) {
        delete position.start;
    }
    if (position.end && position.end->isTemporary()) {
        delete position.end;
    }
}

bool Car::doTurn(bool left)
{
    const qreal TURN_ANGLE = 30;

    // уже перестраиваетсся
    if (isDoingTurn != -1) {
        return false;
    }
    // перестраиваться можно только на ходу
    if (velocity < 15) {
        return false;
    }

    auto sidePoints = position.end->getSidePoints();
    if ((left && !sidePoints.left) || (!left && !sidePoints.right)) {
        return false;
    }

    // нет ли сбоку авто
    int collisionSideHit = parent->isCarsInCollision(getSideCollisions(left), this);
    if (collisionSideHit < 5 && collisionSideHit != -1) {
        return false;
    }

    QPointF endDelta = *position.end - position.current;
    qreal distanceToEnd = std::sqrt(endDelta.x() * endDelta.x() + endDelta.y() * endDelta.y());

    auto sidePoint = left ? sidePoints.left : sidePoints.right;
    QPointF delta = *sidePoint - *position.end;

    // растояние между конечными точками
    qreal endLength = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());

    // длинна пути перестроения
    qreal length = endLength / sin(TURN_ANGLE * TO_RADIANS);

    // Конец ближе, чем конец перестроения
    if (length * cos(TURN_ANGLE * TO_RADIANS) * 1.2 > distanceToEnd) {
        return false;
    }

    // начальный угол
    qreal startAngle = getAngleBetweenPoints(*position.start, *position.end);

    // общий угол
    qreal angle = (startAngle + TURN_ANGLE * (left ? 1 : -1)) * TO_RADIANS;

    auto *newStartPoint = new TrafficPathPoint{position.current};
    newStartPoint->setIsTemporary(true);

    int vId = sidePoint->getId().vId - 2;
    int gId = sidePoint->getId().gId;

    QPointF newEndPosition = position.current + QPointF{
        length * std::cos(angle),
        -length * std::sin(angle)
    };
    auto *newEndPoint = new TrafficPathPoint{newEndPosition.x(), newEndPosition.y()};
    newEndPoint->setIsTemporary(true);
    newEndPoint->setId(
        vId,
        gId
    );

    updatePosition(newStartPoint, newEndPoint);
    updateRotation();

    path = parent->getTrafficPaths()[gId];
    pointIndex = vId;
    isDoingTurn = left;

    return true;
}

void Car::mouseDoubleClickEvent(QMouseEvent *)
{
    qDebug() << "Double click" << this;
    parent->removeCar(this);
}

void Car::setRotationAcceleration(qreal newRotationAcceleration)
{
    rotationAcceleration = newRotationAcceleration;
}

void Car::setVelocityMax(qreal m)
{
    velocityMax = m;
}


