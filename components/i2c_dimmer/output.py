import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import CONF_ID, CONF_ADDRESS

i2c_dimmer_ns = cg.esphome_ns.namespace("i2c_dimmer")
I2CDimmer = i2c_dimmer_ns.class_("I2CDimmer", output.FloatOutput, cg.Component)

CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
    {
        cv.Required(CONF_ID): cv.declare_id(I2CDimmer),
        cv.Required(CONF_ADDRESS): cv.i2c_address,  # I2C-Adresse hinzufügen
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_ADDRESS])
    await output.register_output(var, config)
    await cg.register_component(var, config)
