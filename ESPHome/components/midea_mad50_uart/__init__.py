import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID, ICON_EMPTY, UNIT_EMPTY


DEPENDENCIES = ["uart"]

CONF_MIDEA_MAD50_UART_ID = "midea_mad50_uart_mideamad50uart_id"

midea_mad50_uart_ns = cg.esphome_ns.namespace("midea_mad50_uart")
MideaMAD50UART = midea_mad50_uart_ns.class_(
    "MideaMAD50UART", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(MideaMAD50UART),
            cv.Optional("uart_handshaking_step", default=0): cv.int_,
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    cg.add(var.set_handShaking_step(config["uart_handshaking_step"]))
    #cg.add(getattr(var, f"Delayms")(1000))
