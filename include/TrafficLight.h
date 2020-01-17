/**
 * @file TrafficLight.h
 * @brief Файл, содержащий описание класса светофора
 */

#pragma once

#include <QLabel>
#include "Interface.h"

class MainWindow;

/**
 * @brief Класс, описывающий светофор
 */
class TrafficLight: public QLabel, public UpdatableImageInterface
{
Q_OBJECT

private:
    QPixmap activeLightPixmap;
    QPixmap backgroundPixmap;
    QPixmap activeLightPedPixmap;
    QPixmap backgroundPedPixmap;

    MainWindow *parent;

    int activeLight = 0;
    int rotation;
    bool isPed;

    QPixmap getActiveLightPixmap(int light);

public:
    /**
     * @brief Конструктор светофора
     * @param rotation Поворот светофора (0, 90, 180, 270)
     * @param isPed Является ли светофор пешеходным
     * @param parent Родитель для созданного (в будущем) QLabel'а
     */
    explicit TrafficLight(int rotation, bool isPed = false, MainWindow *parent = nullptr);

    /**
     * @brief Метод, устанавливающий сигнал светофора
     * @param light Индекс сигнала
     */
    void setActiveLight(int light);

    /**
     * @brief Геттер текущего сигнала светофора
     * @return Текущий сигнал светофора
     */
    int getActiveLight();

    void updateImage() override;
};