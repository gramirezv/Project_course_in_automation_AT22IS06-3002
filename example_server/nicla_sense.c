#include "nicla_sense.h"

NICLA_SENSE_BOARD_t  my_nicla_board = {
    
    // board_accelerometer_x,6133,Variable
    .accelerometer.x.node_id = 6133,
    .accelerometer.x.value = 0,   // int16

    // board_accelerometer_y,6136,Variable
    .accelerometer.y.node_id = 6136,
    .accelerometer.y.value = 0,   // int16
      
    // board_accelerometer_z,6139,Variable
    .accelerometer.z.node_id = 6139,
    .accelerometer.z.value = 0,   // int16

    // board_bsec_accuracy,6142,Variable
    .bsec.accuracy.node_id = 6142,
    .bsec.accuracy.value = 0,     // uint8, Byte in OPC UA

    // board_bsec_b_voc_eq,6145,Variable
    .bsec.b_voc_eq.node_id = 6145,
    .bsec.b_voc_eq.value = 0.0,   // float

    // board_bsec_co2_eq,6148,Variable
    .bsec.co2_eq.node_id = 6148,
    .bsec.co2_eq.value = 0,       // uint16

    // board_bsec_comp_g,6151,Variable
    .bsec.comp_g.node_id = 6151,
    .bsec.comp_g.value = 0,       // uint16

    // board_bsec_comp_h,6154,Variable
    .bsec.comp_h.node_id = 6154,
    .bsec.comp_h.value = 0.0,     // float

    // board_bsec_comp_t,6157,Variable
    .bsec.comp_t.node_id = 6157,
    .bsec.comp_t.value = 0.0,     // float

    // board_bsec_iaq,6160,Variable
    .bsec.iaq.node_id = 6160,
    .bsec.iaq.value = 0,          // unit16

    // board_bsec_iaq_s,6163,Variable
    .bsec.iaq_s.node_id = 6163,
    .bsec.iaq_s.value = 0,        // uint16

    // board_gas_value,6166,Variable
    .gas.node_id = 6166,
    .gas.value = 0.0,             //float
    
    // board_gyroscope_x,6169,Variable
    .gyroscope.x.node_id = 6169,
    .gyroscope.x.value = 0.0,     // int16

    // board_gyroscope_y,6172,Variable
    .gyroscope.y.node_id = 6172,
    .gyroscope.y.value = 0.0,     // int16

    // board_gyroscope_z,6175,Variable
    .gyroscope.z.node_id = 6175,
    .gyroscope.z.value = 0.0,     // int16

    // board_humidity_value,6178,Variable
    .humidity.node_id = 6178,
    .humidity.value = 0.0,        // float

    // board_magnetometer_x,6181,Variable
    .magnetometer.x.node_id = 6181,
    .magnetometer.x.value = 0.0,  // int16

    // board_magnetometer_y,6184,Variable
    .magnetometer.y.node_id = 6184,
    .magnetometer.y.value = 0.0,  // int16

    // board_magnetometer_z,6187,Variable
    .magnetometer.z.node_id = 6187,
    .magnetometer.z.value = 0.0,  // int16

    // board_pressure_value,6190,Variable
    .pressure.node_id = 6190,
    .pressure.value = 0.0,        // float

    // board_quaternion_w,6193,Variable
    .quaternion.w.node_id = 6193,
    .quaternion.w.value = 0.0,    // float

    // board_quaternion_x,6196,Variable
    .quaternion.x.node_id = 6196,
    .quaternion.x.value = 0.0,    // float

    // board_quaternion_y,6199,Variable
    .quaternion.y.node_id = 6199,
    .quaternion.y.value = 0.0,    // float

    // board_quaternion_z,6202,Variable
    .quaternion.z.node_id = 6202,
    .quaternion.z.value = 0.0,    // float

    // board_temperature_value,6205,Variable
    .temperature.node_id = 6205,  
    .temperature.value = 0.0,     // float

};