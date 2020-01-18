#include <QTest>
#include "Car.h"
#include "CarFactory.h"
#include "TrafficLightSystem.h"

class Tests: public QObject
{
Q_OBJECT

    TrafficPath *path = nullptr;

    TrafficPathPoint *pointStart = nullptr;
    TrafficPathPoint *pointEnd = nullptr;

    TrafficLight *light = nullptr;

    TrafficLightSystem *lightSystem = nullptr;

private slots:
    static void carFactoryTest()
    {
        QCOMPARE(CarFactory::carData.empty(), false);
    }

    void createTrafficPathPointTest()
    {
        pointEnd = new TrafficPathPoint{1., 1.};
        pointStart = new TrafficPathPoint{QPoint{1, 0}};
    }

    void createTrafficPathTest()
    {
        path = new TrafficPath{};
    }

    void addTrafficPathPointToTrafficPathTest()
    {
        path->addPoint(pointEnd, 1);
        path->addPoint(pointStart, 0);
    }

    void compareTrafficPathPointsTest()
    {
        const auto &points = path->getPoints();

        QCOMPARE(points[0], pointStart);
        QCOMPARE(points[1], pointEnd);
    }

    void carSetMaxVelocityTest()
    {
        auto *car = CarFactory::createCar(nullptr, path);

        delete car;
    }

    void createLightTest()
    {
        light = new TrafficLight{90};
    }

    void setActiveLightCorrectTest()
    {
        light->setActiveLight(1);
        QCOMPARE(light->getActiveLight(), 1);
    }

    void setActiveLightBadTest()
    {
        light->setActiveLight(900);
        QCOMPARE(light->getActiveLight(), 1);
    }

    void createLightSystemTest()
    {
        lightSystem = new TrafficLightSystem{};
    }

    void setLightSystemResetTimeTest()
    {
        lightSystem->setResetTime(100, 100, 100);

        QCOMPARE(lightSystem->getPedTime(), 100);
    }

    void addLightSystemLightTest()
    {
        lightSystem->addLight(light, 2);

        const auto &lights = lightSystem->getLights();

        QCOMPARE(lights[0], static_cast<TrafficLight *>(nullptr));
        QCOMPARE(lights[1], static_cast<TrafficLight *>(nullptr));
        QCOMPARE(lights[2], light);
    }

};

QTEST_MAIN(Tests)
#include "tests.moc"