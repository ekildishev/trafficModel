/**
 * @file Bus.h
 * @brief Файл, содержащий описание класса автобуса и смежных с ним
 */

#pragma once

#include "Car.h"
#include "PedSystem.h"

/**
 * @brief Система пешеходов для автобуса
 */
class BusPedSystem: public PedSystem
{
public:
    using PedSystem::PedSystem;

    void updateData(int msec) override;
};

/**
 * @brief Класс, описывающий автобус
 */
class Bus: public Car
{
    int stopTimeReset = 3500;
    int stopTime = -1;
    bool isStop = false;
    bool stopDisable = false;

    BusPedSystem *pedSystem = new BusPedSystem{
        true,
        {0, 0, 0, 0},
        nullptr,
        parent
    };

protected:
    void updateSpeed(int msec) override;

    void setNewIndex(int index) override;

public:
    /**
     * @brief Конструктор автобуса
     * @param picture Текстура автобуса
     * @param size Размер автобуса
     * @param path Путь для автобуса
     * @param startIndex Индекс стартовой вершины автобуса
     * @param parent Родитель для созданного (в будущем) QLabel'а
     */
    Bus(QPixmap *picture, const QRect &size, TrafficPath *path, int startIndex, MainWindow *parent);

    void updateData(int msec) override;

    void updateImage() override;

    ~Bus() override;
};