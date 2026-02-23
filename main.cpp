#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gpio/gpiofactory.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Create the HAL pin (offset 10 is just an example - change as needed)
    Gpio::IGpioPin* gpioPin = Gpio::GpioFactory::createPin(14);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gpioPin", gpioPin);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("gpio_stmp_demo", "Main");

    return app.exec();
}
