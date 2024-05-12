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
static void stopHandler(int sign) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "received ctrl-c");
    running = false;
}

/** @brief Example how to update pressure value */
static void
updateTemperatureValue(UA_Server *server, void *data) {
    
    // Assign node id for temperature value node, value initialized in nicla_sense.c
    UA_NodeId temperatureNodeId = UA_NODEID_NUMERIC(2, my_nicla_board.temperature.node_id);
    
    // increase temperature by 2 every time this function is called
    my_nicla_board.temperature.value += 5;

    // cast value to an OPC UA variant (TODO: check if this is needed)
    UA_Variant myVar;
    UA_Variant_init(&myVar);
    UA_Variant_setScalar(&myVar, &(my_nicla_board.temperature.value), &UA_TYPES[UA_TYPES_FLOAT]);
    
    // write value to the temperature node
    UA_Server_writeValue(server, temperatureNodeId, myVar);

}

/** @brief Example how to update pressure value */
static void
updatePressureValue(UA_Server *server, void *data) {
    // Assign node id for pressure value node, value initialized in nicla_sense.c
    UA_NodeId pressureNodeId = UA_NODEID_NUMERIC(2, my_nicla_board.pressure.node_id);
    
    // increase pressure by 2.4 every time this function is called
    my_nicla_board.pressure.value += 2.5;

    // cast value to an OPC UA variant (TODO: check if this is needed)
    UA_Variant myVar;
    UA_Variant_init(&myVar);
    UA_Variant_setScalar(&myVar, &(my_nicla_board.pressure.value), &UA_TYPES[UA_TYPES_FLOAT]);
    
    // write value to the pressure node
    UA_Server_writeValue(server, pressureNodeId, myVar);
}

int main(int argc, const char *argv[]) {
    signal(SIGINT, stopHandler);
    signal(SIGTERM, stopHandler);

    UA_Server *server = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(server));

    for (int cnt = 1; cnt < argc; cnt++) {
        if (UA_StatusCode_isBad(UA_Server_loadNodeset(server, argv[cnt], NULL))) {
            printf("Nodeset %s could not be loaded, exit\n", argv[cnt]);
            return EXIT_FAILURE;
        }
    }

    /* Add a repeated callback to the server  to write temperature value */
    UA_Server_addRepeatedCallback(server, updateTemperatureValue, NULL, 2000, NULL); /* call every 2 sec */
    /* Add a repeated callback to the server  to write pressure value */
    UA_Server_addRepeatedCallback(server, updatePressureValue, NULL, 2000, NULL); /* call every 2 sec */
    
    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_Server_delete(server);

    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}
