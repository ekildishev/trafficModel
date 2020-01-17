/**
 * @file Car.h
 * @brief Файл, содержащий описание класса машины и смежных с ним
 */

#pragma once

#include <QLabel>
#include "Interface.h"
#include "TrafficPath.h"

class MainWindow;

/**
 * @brief Класс, описывающий машину
 */
class Car: public QLabel, public UpdatableDataInterface, public UpdatableImageInterface
{
    QPixmap *picture;
    TrafficPath *path;
    int pointIndex;

    QVector<QLabel *> debugHelp;

public:
    /**
     * @brief Структура, описывающая круг
     */
    struct Circle
    {
        QPointF point{0, 0};        ///< Центр круга
        qreal radius{0};                        ///< Радиус
    };

protected:
    /**
     * @brief Событие двойного нажатия левой кнопки мыши по машине
     * @param event Данные о событии
     */
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    /**
     * @brief Получение коллизий, которые находятся спереди
     * @return Вектор из коллизий
     */
    virtual QVector<Circle> getFrontCollisions() const;

    /**
     * @brief Получение коллизий, которые находятся сбоку
     * @param left true, если необходимо получить коллизии слева. Иначе false
     * @return Вектор из коллизий
     */
    virtual QVector<Circle> getSideCollisions(bool left) const;

    /**
     * @brief Обновление скорости машины
     * @param msec Время в миллисекундах, прошедшее с момента последнего обновления скорости
     */
    virtual void updateSpeed(int msec);

    /**
     * @brief Установка нового индекса конечной точки
     * @param index Новый индекс
     */
    virtual void setNewIndex(int index);

    /**
     * @brief Обновление данных о передвижении машины
     * @param start Начальная точка
     * @param end Конечная точка
     */
    void updatePosition(TrafficPathPoint *start, TrafficPathPoint *end);

    /**
     * @brief Обновление угла поворота машины
     */
    void updateRotation();

    /**
     * @brief Очистка объектов, связанных с отладкой
     */
    void clearDebugHelp();

    struct
    {
        TrafficPathPoint *start = nullptr;
        TrafficPathPoint *end = nullptr;

        TrafficPathPoint current;
        qreal distance = 999999;
    } position;                         ///< Данные о передвижении машины
    QRect size;                         ///< Размер машины

    qreal rotation = 0;                 ///< Угол поворота
    qreal rotationAcceleration = 1;     ///< Скорость изменения угла
    qreal rotationMax = 0;              ///< Предельный угол поворота

    qreal velocity = 0;                 ///< Скорость передвижения
    qreal acceleration = 35;            ///< Ускорение
    qreal decceleration = 35;           ///< Замедление
    qreal velocityMax = 50;             ///< Предельная скорость

    MainWindow *parent;                 ///< Родительский объект

    int isDoingTurn = -1;           ///< Выполняет ли машина перестроение. -1 нет, 0 вправо, 1 влево

public:
    /**
     * @brief Конструктор машины
     * @param picture Текстура машины
     * @param size Размер машины
     * @param path Путь машины
     * @param startIndex Индекс стартовой точки пути
     * @param parent Родитель для созданного (в будущем) QLabel'а
     */
    Car(QPixmap *picture, QRect size, TrafficPath *path, int startIndex, MainWindow *parent = nullptr);

    void updateData(int msec) override;

    void updateImage() override;

    /**
     * @brief Сеттер для скорости изменения угла
     * @param rotationAcceleration Новое значение
     */
    void setRotationAcceleration(qreal rotationAcceleration);

    /**
     * @brief Сеттер для предельной скорости
     * @param m Новое значение
     */
    void setVelocityMax(qreal m);

    /**
     * @brief Геттер для текущих коллизий машины
     * @return Вектор из коллизий
     */
    virtual QVector<Circle> getCollisions() const;

    /**
     * @brief Основные точки соприкосновения (чтобы проверять коллизии)
     * @return Вектор точек
     */
    virtual QVector<QPointF> getPivots() const;

    /**
     * @brief Выполнить перестроение, если возможно
     * @param left true, если перестроение выполняется влево. false иначе
     * @return true, если перестроение возможно. false иначе
     */
    virtual bool doTurn(bool left);

    ~Car() override;
};