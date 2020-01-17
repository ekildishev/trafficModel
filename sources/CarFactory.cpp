#include "CarFactory.h"

const QVector<CarFactory::CarData> CarFactory::carData{
    {
        ":/image/CarTruck.png",
        {0, 0, 38, 22},
        {50, 70},
    },
    {
        ":/image/CarSpeed.png",
        {0, 0, 30, 18},
        {70, 80},
    },
    {
        ":/image/CarDefault.png",
        {0, 0, 30, 18},
        {35, 60},
    },
};

Car *CarFactory::createCar(MainWindow *parent, TrafficPath *path)
{
    std::mt19937 generator{std::random_device{}()};
    std::uniform_int_distribution<int> carIndexGenerator{0, carData.size() - 1};

    size_t carIndex = carIndexGenerator(generator);

    auto car = new Car{
        new QPixmap{carData[carIndex].pixmapPath},
        carData[carIndex].size,
        path,
        0,
        parent
    };

    std::uniform_real_distribution<qreal> randomVelocity(
        carData[carIndex].randomVelocityRange.first,
        carData[carIndex].randomVelocityRange.second
    );
    car->setVelocityMax(randomVelocity(generator));

    return car;
}

