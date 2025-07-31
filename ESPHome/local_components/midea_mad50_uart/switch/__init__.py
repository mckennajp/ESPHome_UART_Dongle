import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import CONF_ICON, CONF_TYPE, ENTITY_CATEGORY_CONFIG,ENTITY_CATEGORY_NONE 

from ..__init__ import CONF_MIDEA_MAD50_UART_ID, MideaMAD50UART, midea_mad50_uart_ns

DEPENDENCIES = ["midea_mad50_uart"]

MideaMAD50UARTSwitch = midea_mad50_uart_ns.class_(
    "MideaMAD50UARTSwitch",
    switch.Switch,
    cg.Component,
    cg.Parented.template(MideaMAD50UART),
)

PowerSwitch = midea_mad50_uart_ns.class_("PowerSwitch", MideaMAD50UARTSwitch)
PumpSwitch = midea_mad50_uart_ns.class_("PumpSwitch", MideaMAD50UARTSwitch)

CONF_POWER_SWITCH = "power_switch"
CONF_PUMP_SWITCH = "pump_switch"
ICON_POWER = "mdi:power"
ICON_PUMP = "mdi:pump"

_SWITCH_SCHEMA = (
    switch.switch_schema(
        MideaMAD50UARTSwitch,
        entity_category=ENTITY_CATEGORY_NONE,
    )
    .extend(
        {
            cv.GenerateID(CONF_MIDEA_MAD50_UART_ID): cv.use_id(MideaMAD50UART),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_POWER_SWITCH: _SWITCH_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(PowerSwitch),
                cv.Optional(CONF_ICON, default=ICON_POWER): cv.icon,
            }
        ),
        CONF_PUMP_SWITCH: _SWITCH_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(PumpSwitch),
                cv.Optional(CONF_ICON, default=ICON_PUMP): cv.icon,
            }
        ),
    }
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_MIDEA_MAD50_UART_ID])
    var = await switch.new_switch(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, parent)
    cg.add(getattr(parent, f"set_{config[CONF_TYPE]}_switch")(var))