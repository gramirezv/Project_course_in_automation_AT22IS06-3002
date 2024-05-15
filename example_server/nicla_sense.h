#ifndef __NICLA_SENSE_H
#define __NICLA_SENSE_H

#include <stdint.h>

/** @brief Nicla sensor uint8 value object */
typedef struct
{
    uint16_t    node_id;
    uint8_t     value;
} NICLA_SENSOR_UINT8_VALUE_t;

/** @brief Nicla sensor uint16 value object */
typedef struct
{
    uint16_t    node_id;
    uint16_t    value;
} NICLA_SENSOR_UINT16_VALUE_t;

/** @brief Nicla sensor int16 value object */
typedef struct
{
    uint16_t    node_id;
    int16_t     value;
} NICLA_SENSOR_INT16_VALUE_t;

/** @brief Nicla sensor uint32 value object */
typedef struct
{
    uint16_t    node_id;
    uint32_t    value;
} NICLA_SENSOR_UINT32_VALUE_t;

/** @brief Nicla sensor float value object */
typedef struct
{
    uint16_t    node_id;
    float       value;
} NICLA_SENSOR_FLOAT_VALUE_t;


/** @brief BSEC sensor object */
typedef struct
{
    NICLA_SENSOR_UINT8_VALUE_t     accuracy; // Byte type in OPC UA
    NICLA_SENSOR_FLOAT_VALUE_t      b_voc_eq;
    NICLA_SENSOR_UINT32_VALUE_t    co2_eq;
    NICLA_SENSOR_UINT32_VALUE_t    comp_g;
    NICLA_SENSOR_FLOAT_VALUE_t      comp_h;
    NICLA_SENSOR_FLOAT_VALUE_t      comp_t;
    NICLA_SENSOR_UINT16_VALUE_t    iaq;
    NICLA_SENSOR_UINT16_VALUE_t    iaq_s;
} NICLA_SENSOR_BSEC_t;

/** @brief Quaternion sensor object */
typedef struct
{
    NICLA_SENSOR_FLOAT_VALUE_t w;
    NICLA_SENSOR_FLOAT_VALUE_t x;
    NICLA_SENSOR_FLOAT_VALUE_t y;
    NICLA_SENSOR_FLOAT_VALUE_t z;
} NICLA_SENSOR_QUATERNION_t;

/** @brief Sensor object */
typedef struct
{
    uint16_t    node_id;
    float     value;
} NICLA_SENSOR_t;

/** @brief XYZ sensor object */
typedef struct
{
    NICLA_SENSOR_INT16_VALUE_t x;
    NICLA_SENSOR_INT16_VALUE_t y;
    NICLA_SENSOR_INT16_VALUE_t z;
} NICLA_SENSOR_XYZ_t;

/** @brief Nicla Sense Me board object */
typedef struct 
{
    NICLA_SENSOR_XYZ_t          accelerometer;
    NICLA_SENSOR_BSEC_t         bsec;
    NICLA_SENSOR_t              gas;
    NICLA_SENSOR_XYZ_t          gyroscope;
    NICLA_SENSOR_t              humidity;
    NICLA_SENSOR_XYZ_t          magnetometer;
    NICLA_SENSOR_t              pressure;
    NICLA_SENSOR_QUATERNION_t   quaternion;
    NICLA_SENSOR_t              temperature;
} NICLA_SENSE_BOARD_t;

extern NICLA_SENSE_BOARD_t my_nicla_board;

#endif /*__NICLA_SENSE_H*/