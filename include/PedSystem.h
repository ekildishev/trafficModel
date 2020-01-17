/**
 * @file PedSystem.h
 * @brief Файл, содержащий описание класса системы пешеходов
 */

#pragma once

#include <QLabel>
#include "TrafficLightSystem.h"

class MainWindow;

/**
 * @brief Класс, описывающий пешеходный переход
 */
class PedSystem: public UpdatableDataInterface, public UpdatableImageInterface
{
    MainWindow *parent = nullptr;
    QPixmap *pedPixmap = new QPixmap{":/image/Ped.png"};
    QLabel *label;

    bool isHorizontal;
    TrafficLightSystem *lightSystem = nullptr;

    struct PedData
    {
        QPointF position;
        QPointF delta;
    };

protected:
    size_t maxPeds = 5;                 ///< Максимальное количество пешеходов
    QList<PedData> peds{};              ///< Данные о пешеходах

    bool isActive = false;              ///< Является ли переход активным
    int activeTime = 0;                 ///< Время, оставшееся до деаактивации

    /**
     * @brief Метод, создающий нового пешехода
     * @return Информация о пешеходе
     */
    PedData spawnPed();

    /**
     * @brief Проверка, является ли пешеход на границе перехода
     * @param ped Проверяемый пешеход
     * @return true, если является. false, иначе
     */
    bool isPedOnBorder(const PedData &ped);

    /**
     * @brief Метод, обновляющий данные о пешеходах
     * @param msec Время в миллисекундах, прошедшее с момента последнего обновления данных о пешеходах
     */
    void updatePedData(int msec);

public:
    /**
     * @brief Конструктор пешеходного перехода
     * @param isHorizontal Является ли переход горизонтальным
     * @param geometry Геометрия перехода
     * @param lightSystem Ассоциированная система светофоров
     * @param parent Родитель для созданного (в будущем) QLabel'а
     */
    PedSystem(bool isHorizontal, QRect geometry, TrafficLightSystem *lightSystem, MainWindow *parent);

    void updateData(int msec) override;

    void updateImage() override;

    /**
     * @brief Метод, активирующий переход
     * @param time Время до деактивации
     */
    void activate(int time);

    /**
     * @brief Геттер для композиционной надписи
     * @return Указатель на QLabel
     */
    QLabel *getLabel();

    ~PedSystem();
};