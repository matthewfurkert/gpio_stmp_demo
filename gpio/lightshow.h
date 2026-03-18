#ifndef LIGHTSHOW_H
#define LIGHTSHOW_H

#include <QObject>
#include <QtQml>
#include <QTimer>
#include <vector>

class Gpio;   // forward declaration (QML works perfectly with this)

class LightShow : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool running READ isRunning NOTIFY runningChanged)
    Q_PROPERTY(Gpio* redGpio    READ redGpio    WRITE setRedGpio    NOTIFY redGpioChanged)
    Q_PROPERTY(Gpio* orangeGpio READ orangeGpio WRITE setOrangeGpio NOTIFY orangeGpioChanged)
    Q_PROPERTY(Gpio* greenGpio  READ greenGpio  WRITE setGreenGpio  NOTIFY greenGpioChanged)

public:
    explicit LightShow(QObject* parent = nullptr);
    ~LightShow() override;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    bool isRunning() const;

    Gpio* redGpio() const { return m_red; }
    Gpio* orangeGpio() const { return m_orange; }
    Gpio* greenGpio() const { return m_green; }

public slots:
    void setRedGpio(Gpio* gpio);
    void setOrangeGpio(Gpio* gpio);
    void setGreenGpio(Gpio* gpio);

signals:
    void runningChanged();
    void redGpioChanged();
    void orangeGpioChanged();
    void greenGpioChanged();

private slots:
    void nextStep();

private:
    bool m_running = false;
    Gpio* m_red    = nullptr;
    Gpio* m_orange = nullptr;
    Gpio* m_green  = nullptr;

    QTimer m_timer;
    size_t m_currentStep = 0;

    struct Step {
        bool red, orange, green;
        int durationMs;
    };

    const std::vector<Step> m_pattern;
};

#endif // LIGHTSHOW_H