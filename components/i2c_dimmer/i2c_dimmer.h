#pragma once
#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "Wire.h"

namespace esphome {
namespace i2c_dimmer {

class I2CDimmer : public output::FloatOutput, public Component {
 public:
  I2CDimmer(uint8_t i2c_address) { this->address_ = i2c_address; }

  void setup() override;
  void write_state(float state) override;

 protected:
  uint8_t address_;
};

}  // namespace i2c_dimmer
}  // namespace esphome
