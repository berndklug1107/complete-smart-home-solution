#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace i2c_dimmer {

class I2CDimmer : public output::FloatOutput, public i2c::I2CDevice, public Component {
 public:
  void setup() override {
    // Initialisierung, falls nötig
  }

  void write_state(float value) override {
    // ESPHome liefert 0.0 bis 1.0 -> Umrechnung auf 0 bis 255
    uint8_t val = (uint8_t) (value * 255);
    this->write(&val, 1);
  }

  void dump_config() override;
};

}  // namespace i2c_dimmer
}  // namespace esphome
