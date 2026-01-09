import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output, i2c
from esphome.const import CONF_ID

DEPENDENCIES = ['i2c']


i2c_dimmer_ns = cg.esphome_ns.namespace('i2c_dimmer')

I2CDimmer = i2c_dimmer_ns.class_('I2CDimmer', output.FloatOutput, i2c.I2CDevice, cg.Component)

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend({
    cv.Required(CONF_ID): cv.declare_id(I2CDimmer),
}).extend(cv.COMPONENT_SCHEMA).extend(i2c.i2c_device_schema(0x10))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)
    await i2c.register_i2c_device(var, config)
