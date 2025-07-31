import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import ICON_CHIP,CONF_ID,ENTITY_CATEGORY_NONE,CONF_TYPE

from ..__init__ import CONF_MIDEA_MAD50_UART_ID, MideaMAD50UART, midea_mad50_uart_ns

MideaMAD50UARTTextSensor = midea_mad50_uart_ns.class_(
    "LD2420TextSensor", 
    text_sensor.TextSensor, 
    cg.Component, 
    cg.Parented.template(MideaMAD50UART),
)

Status = midea_mad50_uart_ns.class_("Status", MideaMAD50UARTTextSensor)

_TEXT_SENSOR_SCHEMA = (
    text_sensor.text_sensor_schema(
        MideaMAD50UARTTextSensor,
        entity_category=ENTITY_CATEGORY_NONE,
        icon=ICON_CHIP
    )
    .extend(
        {
            cv.GenerateID(CONF_MIDEA_MAD50_UART_ID): cv.use_id(MideaMAD50UART),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONF_STATUS = "status"

CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_STATUS: _TEXT_SENSOR_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(Status),
            }
        )
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MIDEA_MAD50_UART_ID])
    var = await text_sensor.new_text_sensor(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, parent)
    cg.add(getattr(parent, f"set_{config[CONF_TYPE]}_text_sensor")(var))