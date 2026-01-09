#include "i2c_dimmer.h"
#include "esphome/core/log.h"

namespace esphome {
namespace i2c_dimmer {

static const char *TAG = "i2c_dimmer";

void I2CDimmer::dump_config() {
    ESP_LOGCONFIG(TAG, "I2C Dimmer:");
    LOG_I2C_DEVICE(this);
}

}  // namespace i2c_dimmer
}  // namespace esphome
