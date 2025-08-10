import esphome.codegen as cg
from esphome.components import number
import esphome.config_validation as cv
from esphome.const import CONF_DEVICE_CLASS, DEVICE_CLASS_HUMIDITY,CONF_TYPE, CONF_ICON, CONF_TYPE, ENTITY_CATEGORY_CONFIG, ENTITY_CATEGORY_NONE 

from ..__init__ import CONF_MIDEA_MAD50_UART_ID, MideaMAD50UART, midea_mad50_uart_ns

DEPENDENCIES = ["midea_mad50_uart"]

MideaMAD50UARTNumber = midea_mad50_uart_ns.class_(
    "MideaMAD50UARTNumber",
    number.Number,
    cg.Component,
    cg.Parented.template(MideaMAD50UART),
)

SetPoint = midea_mad50_uart_ns.class_("SetPoint", MideaMAD50UARTNumber)

_NUMBER_SCHEMA = (
    number.number_schema(
        MideaMAD50UARTNumber,
        entity_category=ENTITY_CATEGORY_NONE,
    )
    .extend(
        {
            cv.GenerateID(CONF_MIDEA_MAD50_UART_ID): cv.use_id(MideaMAD50UART),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)
CONF_SETPOINT = "set_point"

CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_SETPOINT: _NUMBER_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(SetPoint),
                cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_HUMIDITY): number.validate_device_class,
            }
        ),
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_MIDEA_MAD50_UART_ID])
    if config[CONF_TYPE] == CONF_SETPOINT:
        var = await number.new_number(config, min_value=35, max_value=70, step=5)
    await cg.register_component(var, config)
    await cg.register_parented(var, parent)
    cg.add(getattr(parent, f"set_{config[CONF_TYPE]}_number")(var))
