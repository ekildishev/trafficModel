#include <QRegularExpression>
#include <QTimer>
#include <cmath>
#include <QKeyEvent>
#include <random>
#include <Bus.h>

#include "Car.h"
#include "TrafficLight.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

bool MainWindow::debugMode = false;

void MainWindow::initGraph()
{
    auto children = this->findChildren<QLabel *>(QRegularExpression{"path_help"});
    for (const auto &child : children) {
        // считываем свойства

        int vId = child->property("vId").toInt();
        int gId = child->property("gId").toInt();
        qreal velocityMax = child->property("velocityMax").toDouble();
        qreal velocity = child->property("velocity").isNull()
                         ? 0 : child->property("velocity").toDouble();
        int lightId = child->property("lightId").toInt();
        int lightGroup = child->property("lightGroup").toInt();
        int spawnRadius = child->property("spawnRadius").toInt();
        bool isBusStop = child->property("busStop").toBool();

        if (!trafficPaths[gId]) {
            trafficPaths[gId] = new TrafficPath{};
        }
        QRect rect = child->geometry();

        auto point = new TrafficPathPoint{
            static_cast<qreal>(rect.x() + rect.width() / 2),
            static_cast<qreal>(rect.y() + rect.height() / 2),
        };
        point->setId(vId, gId);
        point->setVelocityMax(velocityMax);
        point->setVelocity(velocity);
        point->setSpawnRadius(spawnRadius);
        point->setIsBusStop(isBusStop);

        if (lightId != -1) {
            point->setLight(trafficLights[lightGroup]->getLights()[lightId]);
        }

        trafficPaths[gId]->addPoint(point, vId);
    }

    // А теперь инициализируем боковые вершины
    for (const auto &child : children) {
        // считываем свойства

        int vId = child->property("vId").toInt();
        int gId = child->property("gId").toInt();

        int leftVId = child->property("leftVId").isNull() ? -1 : child->property("leftVId").toInt();
        int leftGId = child->property("leftGId").isNull() ? -1 : child->property("leftGId").toInt();
        int rightVId = child->property("rightVId").isNull() ? -1 : child->property("rightVId").toInt();
        int rightGId = child->property("rightGId").isNull() ? -1 : child->property("rightGId").toInt();

        auto &point = trafficPaths[gId]->getPoints()[vId];
        point->setSidePoints(
            leftVId == -1 ? nullptr : trafficPaths[leftGId]->getPoints()[leftVId],
            rightVId == -1 ? nullptr : trafficPaths[rightGId]->getPoints()[rightVId]
        );
    }

}

void MainWindow::initLights()
{
    auto children = this->findChildren<QLabel *>(QRegularExpression{"light_help"});
    for (const auto &child : children) {

        int id = child->property("id").toInt();
        bool isPed = child->property("isPed").toBool();
        int group = child->property("group").toInt();
        qreal rotation = child->property("rotation").toDouble();

        auto *light = new TrafficLight{
            static_cast<int>(rotation),
            isPed,
            this
        };


        light->setGeometry(child->geometry());
        light->updateImage();

        if (!trafficLights[group]) {
            trafficLights[group] = new TrafficLightSystem{};
        }
        if (!isPed) {
            trafficLights[group]->addLight(light, id);
        } else {
            trafficLights[group]->addPedLight(light);
        }

        lights.push_back(light);
    }

    trafficLights[0]->setResetTime(10000, 10000, 10000);
    trafficLights[1]->setResetTime(5000, 10000, 6000);
    trafficLights[2]->setResetTime(5000, 10000, 7000);
    trafficLights[3]->setResetTime(5000, 10000, 5000);
    trafficLights[4]->setResetTime(5000, 6000, 4000);
}

void MainWindow::initPeds()
{
    auto children = this->findChildren<QLabel *>(QRegularExpression{"ped_help"});
    for (const auto &child : children) {

        bool isHorizontal = child->property("isHorizontal").toBool();
        int group = child->property("group").toInt();

        auto *pedSystem = new PedSystem{
            isHorizontal,
            child->geometry(),
            trafficLights[group],
            this
        };
        pedSystems.push_back(pedSystem);
    }
}

void MainWindow::initBackground()
{
    background = new QLabel{this};
    background->setGeometry(0, 0, 800, 600);

    background->setPixmap(QPixmap{":/image/Background.png"});
    background->show();
    background->lower();

    QSize size{background->rect().width(), background->rect().height()};
    this->setMaximumSize(size);
    this->setMinimumSize(size);
}

MainWindow::MainWindow()
    : ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initBackground();
    initLights();
    initGraph();
    initPeds();

    auto *timer = new QTimer{this};
    int interval = 1000 / 60;
    timer->setInterval(interval);

    connect(timer, &QTimer::timeout, [this, interval]() {
        for (TrafficLightSystem *system: trafficLights) {
            if (!system) {
                break;
            }

            system->updateData(interval);
        }
        for (Car *car: cars) {
            car->updateData(interval);
        }
        for (PedSystem *ped: pedSystems) {
            ped->updateData(interval);
        }

        // TODO: унести отсюда
        for (Car *car: cars) {
            car->updateImage();
        }
        for (TrafficLight *light: lights) {
            light->updateImage();
        }
        for (PedSystem *ped: pedSystems) {
            ped->updateImage();
        }
        this->updateImage();
    });
    timer->start();


    auto *turnTimer = new QTimer{this};
    turnTimer->setInterval(5000);                       // попытка перестроения каждые 5 секунд
    connect(turnTimer, &QTimer::timeout, [this, interval]() {
        std::vector<Car *> shuffleCars(cars.cbegin(), cars.cend());

        std::shuffle(
            shuffleCars.begin(),
            shuffleCars.end(),
            std::mt19937{std::random_device{}()}
        );

        for (Car *car : shuffleCars) {
            for (bool left : {true, false}) {
                if (car->doTurn(left)) {
                    return;
                }
            }
        }
    });
    turnTimer->start();

    // Adds new car every 1.5 second
    auto *carTimer = new QTimer{this};
    carTimer->setInterval(1500);

    connect(carTimer, &QTimer::timeout, [this, carTimer]() {
        TrafficPath *path = nullptr;

        size_t attempts = 0;
        while (path == nullptr && attempts < 10) {
            std::mt19937 generator{std::random_device{}()};
            std::uniform_int_distribution<size_t> random(0, trafficPaths.size() - 1);

            path = trafficPaths[random(generator)];
            if (!path) {
                continue;
            }

            auto point = path->getPoints()[0];
            Car::Circle collision{
                *dynamic_cast<QPointF *>(point),
                static_cast<qreal>(point->getSpawnRadius())
            };
            if (isCarsInCollision({collision}, nullptr) != -1) {
                path = nullptr;
                ++attempts;
            }
        }

        if (!path) {
            return;
        }
        if (path == trafficPaths[3]) {
            auto car = new Bus{
                new QPixmap{":/image/Bus.png"},
                {0, 0, 64, 28},
                path,
                0,
                this
            };
            car->show();
            cars.push_back(car);

            return;
        }

        auto car = new Car{
            new QPixmap{":/image/CarTruck.png"},
            {0, 0, 38, 22},
            path,
            0,
            this
        };

        std::mt19937 generator{std::random_device{}()};
        std::uniform_real_distribution<qreal> randomVelocity(60, 75);
        car->setVelocityMax(randomVelocity(generator));

        car->show();
        cars.push_back(car);

    });
    carTimer->start();
}

int MainWindow::isCarsInCollision(const QVector<Car::Circle> &circles, Car *except)
{
    for (size_t i = 0; i < circles.size(); ++i) {
        for (Car *car: cars) {
            if (car == except) {
                continue;
            }

            auto pivots = car->getPivots();
            for (const auto &pivot :pivots) {
                QPointF delta = pivot - circles[i].point;

                qreal length = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
                if (length <= circles[i].radius) {
                    return i;
                }
            }
        }
    }

    return -1;
}

void MainWindow::removeCar(Car *car)
{
    cars.removeOne(car);
    car->deleteLater();
}

const QVector<TrafficPath *> &MainWindow::getTrafficPaths() const
{
    return trafficPaths;
}

void MainWindow::updateImage()
{
    auto children = this->findChildren<QLabel *>(QRegularExpression{"help"});
    for (const auto &child : children) {
        child->setVisible(debugMode);
    }
}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);

    if (event->key() == Qt::Key_F10) {
        debugMode = !debugMode;
    }
}
