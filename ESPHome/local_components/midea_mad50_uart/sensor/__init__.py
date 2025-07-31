import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_NONE, DEVICE_CLASS_HUMIDITY, UNIT_PERCENT,CONF_TYPE,STATE_CLASS_MEASUREMENT,CONF_DEVICE_CLASS
from ..__init__ import CONF_MIDEA_MAD50_UART_ID, MideaMAD50UART, midea_mad50_uart_ns

DEPENDENCIES = ["midea_mad50_uart"]




MideaMAD50UARTSensor = midea_mad50_uart_ns.class_(
    "MideaMAD50UARTSensor",
    sensor.Sensor,
    cg.Component,
    cg.Parented.template(MideaMAD50UART),
)

Humidity = midea_mad50_uart_ns.class_("Humidity", MideaMAD50UARTSensor)

_SENSOR_SCHEMA = (
    sensor.sensor_schema(
        MideaMAD50UARTSensor,
        entity_category=ENTITY_CATEGORY_NONE,
        unit_of_measurement = UNIT_PERCENT,
        accuracy_decimals = 0,
        device_class=DEVICE_CLASS_HUMIDITY,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.GenerateID(CONF_MIDEA_MAD50_UART_ID): cv.use_id(MideaMAD50UART),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONF_HUMIDITY = "humidity"

CONFIG_SCHEMA = cv.typed_schema(
    {
        CONF_HUMIDITY: _SENSOR_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(Humidity),
                cv.Optional(CONF_DEVICE_CLASS, default=DEVICE_CLASS_HUMIDITY): sensor.validate_device_class,
            }
           
        )
    }
)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_MIDEA_MAD50_UART_ID])
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await cg.register_parented(var, parent)
    cg.add(getattr(parent, f"set_{config[CONF_TYPE]}_sensor")(var))