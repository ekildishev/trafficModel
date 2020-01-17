/**
 * @file TrafficPath.h
 * @brief Файл, содержащий описание класса пути
 */

#pragma once

#include "TrafficPathPoint.h"

/**
 * @brief Класс, описывающий путь
 */
class TrafficPath
{
    QVector<TrafficPathPoint *> points{50, nullptr};

public:
    /**
     * @brief Геттер вершин
     * @warning Указатель на вершину может быть nullptr
     * @return Вектор из указателей на вершины
     */
    const QVector<TrafficPathPoint *> &getPoints() const;

    /**
     * @brief Добавление вершины по индексу
     * @param point Указатель на вершину
     * @param index Индекс
     */
    void addPoint(TrafficPathPoint *point, int index);

    ~TrafficPath();
};