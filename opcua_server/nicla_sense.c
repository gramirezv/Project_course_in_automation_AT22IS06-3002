#include "nicla_sense.h"

NICLA_SENSE_BOARD_t  my_nicla_board = {
    
    // board_accelerometer_x
    .accelerometer.x.node_id = "accelerometer.x",
    .accelerometer.x.value = 0,   // int16

    // board_accelerometer_y
    .accelerometer.y.node_id = "accelerometer.y",
    .accelerometer.y.value = 0,   // int16
      
    // board_accelerometer_z
    .accelerometer.z.node_id = "accelerometer.z",
    .accelerometer.z.value = 0,   // int16

    // board_bsec_accuracy
    .bsec.accuracy.node_id = "bsec.accuracy",
    .bsec.accuracy.value = 0,     // uint8, Byte in OPC UA

    // board_bsec_b_voc_eq
    .bsec.b_voc_eq.node_id = "bsec.b_voc_eq",
    .bsec.b_voc_eq.value = 0.0,   // float

    // board_bsec_co2_eq
    .bsec.co2_eq.node_id = "bsec.co2_eq",
    .bsec.co2_eq.value = 0,       // uint16

    // board_bsec_comp_g
    .bsec.comp_g.node_id = "bsec.comp_g",
    .bsec.comp_g.value = 0,       // uint16

    // board_bsec_comp_h
    .bsec.comp_h.node_id = "bsec.comp_h",
    .bsec.comp_h.value = 0.0,     // float

    // board_bsec_comp_t
    .bsec.comp_t.node_id = "bsec.comp_t",
    .bsec.comp_t.value = 0.0,     // float

    // board_bsec_iaq
    .bsec.iaq.node_id = "bsec.iaq",
    .bsec.iaq.value = 0,          // unit16

    // board_bsec_iaq_s
    .bsec.iaq_s.node_id = "bsec.iaq_s",
    .bsec.iaq_s.value = 0,        // uint16

    // board_gas_value
    .gas.node_id = "gas",
    .gas.value = 0.0,             //float
    
    // board_gyroscope_x
    .gyroscope.x.node_id = "gyroscope.x",
    .gyroscope.x.value = 0.0,     // int16

    // board_gyroscope_y
    .gyroscope.y.node_id = "gyroscope.y",
    .gyroscope.y.value = 0.0,     // int16

    // board_gyroscope_z
    .gyroscope.z.node_id = "gyroscope.z",
    .gyroscope.z.value = 0.0,     // int16

    // board_humidity_value
    .humidity.node_id = "humidity",
    .humidity.value = 0.0,        // float

    // board_magnetometer_x
    .magnetometer.x.node_id = "magnetometer.x",
    .magnetometer.x.value = 0.0,  // int16

    // board_magnetometer_y
    .magnetometer.y.node_id = "magnetometer.y",
    .magnetometer.y.value = 0.0,  // int16

    // board_magnetometer_z
    .magnetometer.z.node_id = "magnetometer.z",
    .magnetometer.z.value = 0.0,  // int16

    // board_pressure_value
    .pressure.node_id = "pressure",
    .pressure.value = 0.0,        // float

    // board_quaternion_w
    .quaternion.w.node_id = "quaternion.w",
    .quaternion.w.value = 0.0,    // float

    // board_quaternion_x
    .quaternion.x.node_id = "quaternion.x",
    .quaternion.x.value = 0.0,    // float

    // board_quaternion_y
    .quaternion.y.node_id = "quaternion.y",
    .quaternion.y.value = 0.0,    // float

    // board_quaternion_z
    .quaternion.z.node_id = "quaternion.z",
    .quaternion.z.value = 0.0,    // float

    // board_temperature_value
    .temperature.node_id = "temperature",  
    .temperature.value = 0.0,     // float

};