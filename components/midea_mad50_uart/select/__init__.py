import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_NONE,CONF_ICON,CONF_TYPE

from ..__init__ import CONF_MIDEA_MAD50_UART_ID, MideaMAD50UART, midea_mad50_uart_ns

DEPENDENCIES = ["midea_mad50_uart"]

MideaMAD50UARTSelect = midea_mad50_uart_ns.class_(
    "MideaMAD50UARTSelect",
    select.Select,
    cg.Component,
    cg.Parented.template(MideaMAD50UART),
)

Mode = midea_mad50_uart_ns.class_("Mode", MideaMAD50UARTSelect)
Fan = midea_mad50_uart_ns.class_("Fan", MideaMAD50UARTSelect)

_SELECT_SCHEMA = (
    select.select_schema(
        MideaMAD50UARTSelect,
        entity_category=ENTITY_CATEGORY_NONE,
    )
    .extend(
        {
            cv.GenerateID(CONF_MIDEA_MAD50_UART_ID): cv.use_id(MideaMAD50UART),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONF_MODE = "mode"
CONF_FAN = "fan"

CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_MODE: _SELECT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(Mode),
                cv.Optional(CONF_ICON, default="mdi:menu"): cv.icon,
            }
        ),
        CONF_FAN: _SELECT_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(Fan),
                cv.Optional(CONF_ICON, default="mdi:fan"): cv.icon,
            }
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MIDEA_MAD50_UART_ID])
    if config[CONF_TYPE] == CONF_MODE:
        var = await select.new_select(config, options=["SetPoint","Continuous","Drying","Smart"])
    if config[CONF_TYPE] == CONF_FAN:
        var = await select.new_select(config, options=["High","Low"])
    await cg.register_component(var, config)
    await cg.register_parented(var, parent)
    cg.add(getattr(parent, f"set_{config[CONF_TYPE]}_select")(var))
