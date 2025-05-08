#include "i2c_dimmer.h"
#include "esphome/core/log.h"

namespace esphome {
namespace i2c_dimmer {

static const char *const TAG = "i2c_dimmer";

void I2CDimmer::setup() {
  Wire.begin();
  ESP_LOGD(TAG, "I2C Dimmer initialized at address 0x%X", this->address_);
}

void I2CDimmer::write_state(float state) {
  uint8_t brightness = state * 255;
  
  Wire.beginTransmission(this->address_);
  Wire.write(brightness);
  Wire.endTransmission();
  
  ESP_LOGD(TAG, "Set brightness to %d on 0x%X", brightness, this->address_);
}

}  // namespace i2c_dimmer
}  // namespace esphome
