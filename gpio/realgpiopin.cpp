#include "realgpiopin.h"
#include <QDebug>

#ifdef USE_REAL_GPIO
#include <gpiod.h>
#endif

namespace Gpio {

RealGpioPin::RealGpioPin(unsigned int offset, QObject* parent)
    : IGpioPin(offset, parent)
    , m_offset(offset)
{
#ifdef USE_REAL_GPIO
    // Prefer explicit path for v2 compatibility (change if you need a different chip, e.g. "/dev/gpiochip4")
    m_chip = gpiod_chip_open("/dev/gpiochip0");
    if (!m_chip) {
        qWarning() << "Failed to open /dev/gpiochip0";
        return;
    }

    struct gpiod_request_config *req_cfg = gpiod_request_config_new();
    if (req_cfg)
        gpiod_request_config_set_consumer(req_cfg, "gpio_hal_hello");

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings) {
        qWarning() << "Failed to create line settings";
        if (req_cfg) gpiod_request_config_free(req_cfg);
        gpiod_chip_close(m_chip);
        m_chip = nullptr;
        return;
    }

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, GPIOD_LINE_VALUE_INACTIVE); // initial value = false / low

    struct gpiod_line_config *line_cfg = gpiod_line_config_new();
    if (!line_cfg) {
        qWarning() << "Failed to create line config";
        gpiod_line_settings_free(settings);
        if (req_cfg) gpiod_request_config_free(req_cfg);
        gpiod_chip_close(m_chip);
        m_chip = nullptr;
        return;
    }

    if (gpiod_line_config_add_line_settings(line_cfg, &m_offset, 1, settings) < 0) {
        qWarning() << "Failed to add line settings for offset" << m_offset;
        gpiod_line_config_free(line_cfg);
        gpiod_line_settings_free(settings);
        if (req_cfg) gpiod_request_config_free(req_cfg);
        gpiod_chip_close(m_chip);
        m_chip = nullptr;
        return;
    }

    m_request = gpiod_chip_request_lines(m_chip, req_cfg, line_cfg);

    // Always free temporary objects
    gpiod_line_config_free(line_cfg);
    gpiod_line_settings_free(settings);
    if (req_cfg)
        gpiod_request_config_free(req_cfg);

    if (m_request) {
        qDebug() << "Real GPIO" << m_offset << "requested successfully (libgpiod v2)";
    } else {
        qWarning() << "Failed to request GPIO line" << m_offset;
        gpiod_chip_close(m_chip);
        m_chip = nullptr;
    }
#else
    qWarning() << "RealGpioPin compiled without USE_REAL_GPIO - this should never happen";
#endif
}

RealGpioPin::~RealGpioPin()
{
#ifdef USE_REAL_GPIO
    if (m_request) {
        gpiod_line_request_release(m_request);
        m_request = nullptr;
    }
    if (m_chip) {
        gpiod_chip_close(m_chip);
        m_chip = nullptr;
    }
#endif
}

bool RealGpioPin::value() const
{
#ifdef USE_REAL_GPIO
    if (m_request) {
        int val = gpiod_line_request_get_value(m_request, m_offset);
        if (val < 0) {
            qWarning() << "Failed to get value for GPIO line" << m_offset;
            return false;
        }
        return val == GPIOD_LINE_VALUE_ACTIVE;
    }
#endif
    return false;
}

void RealGpioPin::setValue(bool value)
{
#ifdef USE_REAL_GPIO
    if (m_request) {
        enum gpiod_line_value val = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
        int ret = gpiod_line_request_set_value(m_request, m_offset, val);
        if (ret < 0) {
            qWarning() << "Failed to set value for GPIO line" << m_offset;
        }
        emit valueChanged(value);
    }
#endif
}

} // namespace Gpio
