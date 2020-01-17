/**
 * @file MainWindow.h
 * @brief Файл, содержащий описание класса основного окна
 */

#pragma once

#include <QMainWindow>
#include <QDebug>
#include <QLabel>

#include "TrafficLightSystem.h"
#include "TrafficPath.h"
#include "PedSystem.h"
#include "Car.h"


namespace Ui
{
class MainWindow;
}

/**
 * @brief Класс, описывающий основное окно
 */
class MainWindow: public QMainWindow, public UpdatableImageInterface
{
Q_OBJECT

private:
    void initBackground();

    void initGraph();

    void initPeds();

    void initLights();

    QList<Car *> cars;
    QVector<PedSystem *> pedSystems;

    QLabel *background;

    QVector<TrafficPath *> trafficPaths{16, nullptr};
    QVector<TrafficLightSystem *> trafficLights{10, nullptr};

    QList<UpdatableImageInterface *> updateImageList{this};
    QList<UpdatableDataInterface *> updateDataList{};

    Ui::MainWindow *ui = nullptr;

protected:
    /**
     * @brief Событие нажатия клавиши
     * @param event Данные о событии
     */
    void keyPressEvent(QKeyEvent *event) override;

public:
    static bool debugMode;          ///< Включен ли отладочный режим

    MainWindow();

    /**
     * @brief Проверяет наличие машин в коллизиях
     * @param circles Упорядоченное множество коллизий
     * @param except Машина, мнение которой не учитывается
     * @return Номер коллизии, в которой нашлась машина
     */
    int isCarsInCollision(const QVector<Car::Circle> &circles, Car *except);

    /**
     * @brief Удаление машины
     * @param car Указатель на машину
     */
    void removeCar(Car *car);

    /**
     * @brief Получение вектора путей
     * @return Вектор путей
     */
    const QVector<TrafficPath *> &getTrafficPaths() const;

    void updateImage() override;

    /**
     * @brief Отрисовка окружности
     * @param circle Данные об окружности
     * @param color Данные о цвете
     * @return Укзатель на QLabel
     */
    QLabel *drawCircle(const Car::Circle &circle, const QColor &color = {255, 171, 0, 150});

    /**
     * @brief Отрисовка квадрата
     * Построение осуществляется по центральной точке и минимальному расстоянию до стороны (радиус)
     * @param circle Данные о квадрате
     * @param color Данные о цвете
     * @return Укзатель на QLabel
     */
    QLabel *drawSquare(const Car::Circle &circle, const QColor &color = {255, 171, 0, 150});

    ~MainWindow() override;
};