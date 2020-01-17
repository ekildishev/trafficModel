/**
 * @file CarFactory.h
 * @brief Файл, содержащий описание класса простой фабрики машин
 */

#pragma once

#include <QVector>
#include <random>

#include "Car.h"

class MainWindow;

/**
 * @brief Простая фабрика машин
 */
class CarFactory
{
    struct CarData {
        QString pixmapPath{};
        QRect size{};
        QPair<qreal, qreal> randomVelocityRange{};
    };

public:
    static const QVector<CarData> carData;          ///< Данные о машинах

    /**
     * @brief Метод, создающий случайную машину
     * @param parent Родительский элемент
     * @param path Путь
     * @return Указатель на Car
     */
    static Car *createCar(MainWindow *parent, TrafficPath *path);
};