/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/nodesetloader.h>

#include <signal.h>
#include <stdio.h>

#include "nicla_sense.h"

UA_Double myTempValue = 0.0;

UA_Boolean running = true;
static void stopHandler(int sign)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

/** @brief Increase Nicla Object values. This fuction is for testing purposes only
 * It will be replaced with values read from the seral port comming from the arduino board
 */
static void
increaseNiclaValues(void) {

    // Accelerometer
    my_nicla_board.accelerometer.x.value += 25;
    my_nicla_board.accelerometer.y.value += 25;
    my_nicla_board.accelerometer.z.value += 25;

    // BSEC
    my_nicla_board.bsec.accuracy.value += 2;
    my_nicla_board.bsec.b_voc_eq.value += 2.5;
    my_nicla_board.bsec.co2_eq.value += 25;
    my_nicla_board.bsec.comp_g.value += 25;
    my_nicla_board.bsec.comp_h.value += 2.5;
    my_nicla_board.bsec.comp_t.value += 2.5;
    my_nicla_board.bsec.iaq.value += 25;
    my_nicla_board.bsec.iaq_s.value += 25;

    // Gas
    my_nicla_board.gas.value += 2.5;

    // Accelerometer
    my_nicla_board.gyroscope.x.value += 25;
    my_nicla_board.gyroscope.y.value += 25;
    my_nicla_board.gyroscope.z.value += 25;

    // Humidity
    my_nicla_board.humidity.value += 2.5;

    // Magnetometer
    my_nicla_board.magnetometer.x.value += 25;
    my_nicla_board.magnetometer.y.value += 25;
    my_nicla_board.magnetometer.z.value += 25;

    // Pressure
    my_nicla_board.pressure.value += 2.5;

    // Quaternion
    my_nicla_board.quaternion.w.value += 2.5;
    my_nicla_board.quaternion.x.value += 2.5;
    my_nicla_board.quaternion.y.value += 2.5;
    my_nicla_board.quaternion.z.value += 2.5;

    // Temperature
    my_nicla_board.temperature.value += 2.5;
}

/** @brief Update Nicla Object value */
static void
writeNiclaValue(UA_Server *server, char *nodeId, void *__restrict__ p, const UA_DataType *type)
{

    UA_Variant myVar;
    UA_Variant_init(&myVar);

    UA_Variant_setScalar(&myVar, p, type);

    UA_Server_writeValue(server, UA_NODEID_STRING(2, nodeId), myVar);
}

/** @brief Update Nicla Board values from interface */
static void
updateValuesFromNicla(UA_Server *server, void *data)
{
    // Added for testing purposes, this should be replaced with the function that reads from the nicla
    increaseNiclaValues();

    // Accelerometer
    writeNiclaValue(server, my_nicla_board.accelerometer.x.node_id, &(my_nicla_board.accelerometer.x.value), &UA_TYPES[UA_TYPES_INT16]);
    writeNiclaValue(server, my_nicla_board.accelerometer.y.node_id, &(my_nicla_board.accelerometer.y.value), &UA_TYPES[UA_TYPES_INT16]);
    writeNiclaValue(server, my_nicla_board.accelerometer.z.node_id, &(my_nicla_board.accelerometer.z.value), &UA_TYPES[UA_TYPES_INT16]);

    // BSEC
    writeNiclaValue(server, my_nicla_board.bsec.accuracy.node_id, &(my_nicla_board.bsec.accuracy.value), &UA_TYPES[UA_TYPES_BYTE]);
    writeNiclaValue(server, my_nicla_board.bsec.b_voc_eq.node_id, &(my_nicla_board.bsec.b_voc_eq.value), &UA_TYPES[UA_TYPES_FLOAT]);
    writeNiclaValue(server, my_nicla_board.bsec.co2_eq.node_id, &(my_nicla_board.bsec.co2_eq.value), &UA_TYPES[UA_TYPES_UINT32]);
    writeNiclaValue(server, my_nicla_board.bsec.comp_g.node_id, &(my_nicla_board.bsec.comp_g.value), &UA_TYPES[UA_TYPES_UINT32]);
    writeNiclaValue(server, my_nicla_board.bsec.comp_h.node_id, &(my_nicla_board.bsec.comp_h.value), &UA_TYPES[UA_TYPES_FLOAT]);
    writeNiclaValue(server, my_nicla_board.bsec.comp_t.node_id, &(my_nicla_board.bsec.comp_t.value), &UA_TYPES[UA_TYPES_FLOAT]);
    writeNiclaValue(server, my_nicla_board.bsec.iaq.node_id, &(my_nicla_board.bsec.iaq.value), &UA_TYPES[UA_TYPES_UINT16]);
    writeNiclaValue(server, my_nicla_board.bsec.iaq_s.node_id, &(my_nicla_board.bsec.iaq_s.value), &UA_TYPES[UA_TYPES_UINT16]);

    // Gas
    writeNiclaValue(server, my_nicla_board.gas.node_id, &(my_nicla_board.gas.value), &UA_TYPES[UA_TYPES_FLOAT]);

    // Accelerometer
    writeNiclaValue(server, my_nicla_board.gyroscope.x.node_id, &(my_nicla_board.gyroscope.x.value), &UA_TYPES[UA_TYPES_INT16]);
    writeNiclaValue(server, my_nicla_board.gyroscope.y.node_id, &(my_nicla_board.gyroscope.y.value), &UA_TYPES[UA_TYPES_INT16]);
    writeNiclaValue(server, my_nicla_board.gyroscope.z.node_id, &(my_nicla_board.gyroscope.z.value), &UA_TYPES[UA_TYPES_INT16]);

    // Humidity
    writeNiclaValue(server, my_nicla_board.humidity.node_id, &(my_nicla_board.humidity.value), &UA_TYPES[UA_TYPES_FLOAT]);

    // Magnetometer
    writeNiclaValue(server, my_nicla_board.magnetometer.x.node_id, &(my_nicla_board.magnetometer.x.value), &UA_TYPES[UA_TYPES_INT16]);
    writeNiclaValue(server, my_nicla_board.magnetometer.y.node_id, &(my_nicla_board.magnetometer.y.value), &UA_TYPES[UA_TYPES_INT16]);
    writeNiclaValue(server, my_nicla_board.magnetometer.z.node_id, &(my_nicla_board.magnetometer.z.value), &UA_TYPES[UA_TYPES_INT16]);

    // Pressure
    writeNiclaValue(server, my_nicla_board.pressure.node_id, &(my_nicla_board.pressure.value), &UA_TYPES[UA_TYPES_FLOAT]);

    // Quaternion
    writeNiclaValue(server, my_nicla_board.quaternion.w.node_id, &(my_nicla_board.quaternion.w.value), &UA_TYPES[UA_TYPES_FLOAT]);
    writeNiclaValue(server, my_nicla_board.quaternion.x.node_id, &(my_nicla_board.quaternion.x.value), &UA_TYPES[UA_TYPES_FLOAT]);
    writeNiclaValue(server, my_nicla_board.quaternion.y.node_id, &(my_nicla_board.quaternion.y.value), &UA_TYPES[UA_TYPES_FLOAT]);
    writeNiclaValue(server, my_nicla_board.quaternion.z.node_id, &(my_nicla_board.quaternion.z.value), &UA_TYPES[UA_TYPES_FLOAT]);

    // Temperature
    writeNiclaValue(server, my_nicla_board.temperature.node_id, &(my_nicla_board.temperature.value), &UA_TYPES[UA_TYPES_FLOAT]);
}

int main(int argc, const char *argv[])
{
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    for (int cnt = 1; cnt < argc; cnt++)
    {
        if (UA_StatusCode_isBad(UA_Server_loadNodeset(server, argv[cnt], NULL)))
        {
            printf("Nodeset %s could not be loaded, exit\n", argv[cnt]);
            return EXIT_FAILURE;
        }
    }

    /* Add a repeated callback to the server  to write temperature value */
    UA_Server_addRepeatedCallback(server, updateValuesFromNicla, NULL, 2000, NULL); /* call every 2 sec */

    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_Server_delete(server);

    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
