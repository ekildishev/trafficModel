/**
 * @file TrafficPathPoint.h
 * @brief Файл, содержащий описание класса вершины пути
 */

#pragma once

#include <QPointF>

#include "TrafficLight.h"

/**
 * @brief Класс, описывающий вершину пути
 */
class TrafficPathPoint: public QPointF
{
    bool isTemporary_ = false;

    TrafficLight *light = nullptr;

    struct
    {
        TrafficPathPoint *left = nullptr;
        TrafficPathPoint *right = nullptr;
    } sidePoint;

    struct {
        int vId = -1;
        int gId = -1;
    } id;

    qreal velocityMax = 999999;
    qreal velocity = 0; // начальная скорость

    int spawnRadius = 10;

    bool isBusStop_ = false;

public:
    using QPointF::QPointF;

    /**
     * @brief Конструктор вершины пути по координатам
     * @param xpos X координата
     * @param ypos Y координата
     */
    TrafficPathPoint(qreal xpos, qreal ypos);

    /**
     * @brief Конструктор копирования
     * @param object Копируемый объект
     */
    TrafficPathPoint(const TrafficPathPoint &object) = default;

    /**
     * @brief Конструктор перемещения
     * @param object Перемещаемый объект
     */
    TrafficPathPoint(TrafficPathPoint &&object) = default;

    /**
     * @brief Присваивающий оператор копирования
     * @param object Копируемый объект
     * @return Ссылка на текущий элемент
     */
    TrafficPathPoint &operator=(const TrafficPathPoint &object) = default;

    /**
     * @brief Присваивающий оператор перемещения
     * @param object Перемещаемый объект
     * @return Ссылка на текущий элемент
     */
    TrafficPathPoint &operator=(TrafficPathPoint &&object) = default;

    /**
     * @brief Является ли вершина временной
     * @return true, если является. false, иначе
     */
    bool isTemporary() const;

    /**
     * @brief Метод, устанавливающий, является ли вершина временной
     * @param isTemporary Устанавливаемое значение
     */
    void setIsTemporary(bool isTemporary);

    /**
     * @brief Получение предельной скорости
     * @return Предельная скорость
     */
    qreal getVelocityMax() const;

    /**
     * @brief Метод, устанавливающий предельную скорость
     * @param velocityMax Устанавливаемое значение
     */
    void setVelocityMax(qreal velocityMax);

    /**
     * @brief Получение вершин из соседних путей
     * @return Структура из указателей на левую и правую вершину
     */
    auto getSidePoints() const -> decltype(sidePoint);

    /**
     * @brief Метод, устанавливающий соседние вершины
     * @param left Указатель на левую вершину
     * @param right Указатель на правую вершину
     */
    void setSidePoints(TrafficPathPoint *left, TrafficPathPoint *right);

    /**
     * @brief Метод, устанавливающий ассоциированный светофор
     * @param light Указатель на светофро
     */
    void setLight(TrafficLight *light);

    /**
     * @brief Получение максимальной скорсоти
     * @return Максимальная скорость
     */
    qreal getVelocity() const;

    /**
     * @brief Метод, устанавливающий максимальную скорость
     * @param velocity Устанавливаемое значение
     */
    void setVelocity(qreal velocity);

    /**
     * @brief Получение радиуса проверки наличия машин перед созданием новой
     * @return Радиус
     */
    int getSpawnRadius() const;

    /**
     * @brief Метод, устанавливающий радиус проверки наличия машин перед созданием новой
     * @param spawnRadius Устанавливаемое значение
     */
    void setSpawnRadius(int spawnRadius);

    /**
     * @brief Метод, устанавливающий идентификаторы вершины
     * @param vId Индекс вершины
     * @param gId Индекс группы вершины
     */
    void setId(int vId, int gId);

    /**
     * @brief Получение идентификаторов вершины
     * @return Структуру с индексом вершины и группы вершины
     */
    auto getId() const -> decltype(id);

    /**
     * @brief Нужно ли машине остановиться
     * @return true, если нужно. false, иначе
     */
    bool isStop() const;

    /**
     * @brief Является ли вершина автобусной остановкой
     * @return true, если является. false, иначе
     */
    bool isBusStop() const;

    /**
     * @brief Метод, устанавливающий, является ли вершина автобусной остановкой
     * @param isBusStop Устанавливаемое значение
     */
    void setIsBusStop(bool isBusStop);
};