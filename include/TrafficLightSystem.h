/**
 * @file TrafficLightSystem.h
 * @brief Файл, содержащий описание класса системы светофором
 */

#pragma once

#include <QObject>
#include "TrafficLight.h"

/**
 * @brief Класс, описывающий систему светофоров
 */
class TrafficLightSystem: public QObject, public UpdatableDataInterface
{
Q_OBJECT

private:
    int timeToNextResetOdd = 6000;
    int timeToNextReset = 6000;
    int timePed = 6000;

    int timeMiddleState = 1000;
    int timeToNext = 0;

    bool nextStateFullRed = false;

    QVector<TrafficLight *> lights{10, nullptr};
    QVector<TrafficLight *> pedLights{};

public:
    /**
     * @brief Конструктор системы светофоров
     */
    explicit TrafficLightSystem() = default;

    void updateData(int msec) override;

    /**
     * @brief Метод, добавляющий светофор по индексу
     * @param light Укзатель на светофор
     * @param index Индекс
     */
    void addLight(TrafficLight *light, int index);

    /**
     * @brief Метод, добавляющий пешеходный светофор
     * @param light Светофор
     */
    void addPedLight(TrafficLight *light);

    /**
     * @brief Геттер светофоров (не являющихся пешеходными)
     * @return Вектор из светофоров
     */
    const QVector<TrafficLight *> &getLights() const;

    /**
     * @brief Метод, устанавливающий время переключения
     * @param odd Для светофоров с четным индексом
     * @param even Для светофоров с нечетным индексом
     * @param ped Для пешеходных светофоров
     */
    void setResetTime(int odd, int even, int ped);

    /**
     * @brief Являются ли в данный момент включенными пешеходные светофоры
     * @return true, если являются. false, иначе
     */
    bool isPedTime() const;

    /**
     * @brief Геттер времени для пешеходных светофоров
     * @return Время в миллисекундах
     */
    int getPedTime();

    ~TrafficLightSystem() override;
};