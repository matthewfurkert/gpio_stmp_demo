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
    Q_PROPERTY(Gpio* light1 READ light1 WRITE setLight1 NOTIFY light1Changed)
    Q_PROPERTY(Gpio* light2 READ light2 WRITE setLight2 NOTIFY light2Changed)
    Q_PROPERTY(Gpio* light3 READ light3 WRITE setLight3 NOTIFY light3Changed)

public:
    explicit LightShow(QObject* parent = nullptr);
    ~LightShow() override;

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();
    bool isRunning() const;

    Gpio* light1() const { return m_light1; }
    Gpio* light2() const { return m_light2; }
    Gpio* light3() const { return m_light3; }

public slots:
    void setLight1(Gpio* gpio);
    void setLight2(Gpio* gpio);
    void setLight3(Gpio* gpio);

signals:
    void runningChanged();
    void light1Changed();
    void light2Changed();
    void light3Changed();

private slots:
    void nextStep();

private:
    bool m_running = false;
    Gpio* m_light1    = nullptr;
    Gpio* m_light2 = nullptr;
    Gpio* m_light3  = nullptr;

    QTimer m_timer;
    size_t m_currentStep = 0;

    struct Step {
        bool firstLight, secondLight, thirdLight;
        int durationMs;
    };

    const std::vector<Step> m_pattern;
};

#endif // LIGHTSHOW_H