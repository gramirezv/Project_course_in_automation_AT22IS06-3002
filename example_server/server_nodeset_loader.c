/* This work is licensed under a Creative Commons CCZero 1.0 Universal License.
 * See http://creativecommons.org/publicdomain/zero/1.0/ for more information. */

#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <open62541/plugin/nodesetloader.h>

#include <signal.h>
#include <stdio.h>

#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define SERIAL_PORT "\\\\.\\COM8"
#else
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#define SERIAL_PORT "/dev/ttyACM0"
#endif

#include "nicla_sense.h"

#define MAX_DATA_LENGTH 1024
#define MAX_TOKENS 25

int read_serial_port(char *buffer, size_t max_len);

#ifdef _WIN32
int read_serial_port(char *buffer, size_t max_len) {
    HANDLE hSerial;
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = {0};

    fprintf(stderr, "Opening serial port...");
    hSerial = CreateFile(
                SERIAL_PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error\n");
        return 0;
    } else {
        fprintf(stderr, "OK\n");
    }

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (GetCommState(hSerial, &dcbSerialParams) == 0) {
        fprintf(stderr, "Error getting device state\n");
        CloseHandle(hSerial);
        return 0;
    }

    dcbSerialParams.BaudRate = CBR_115200;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (SetCommState(hSerial, &dcbSerialParams) == 0) {
        fprintf(stderr, "Error setting device parameters\n");
        CloseHandle(hSerial);
        return 0;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (SetCommTimeouts(hSerial, &timeouts) == 0) {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 0;
    }

    // Flush any existing data in the buffer
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    DWORD dwBytesRead = 0;
    char c;
    int idx = 0;
    int found_newline = 0;

    // Read until we encounter a newline character indicating start of data
    while (!found_newline && ReadFile(hSerial, &c, 1, &dwBytesRead, NULL) && dwBytesRead == 1) {
        if (c == '\n') {
            found_newline = 1;
        }
    }

    // Now read the actual data line
    if (found_newline) {
        idx = 0;
        while (idx < max_len && ReadFile(hSerial, &c, 1, &dwBytesRead, NULL) && dwBytesRead == 1) {
            buffer[idx++] = c;
            if (c == '\n') {
                break;
            }
        }
        buffer[idx] = '\0'; // Null-terminate the string
    }

    fprintf(stderr, "Closing serial port...");
    if (CloseHandle(hSerial) == 0) {
        fprintf(stderr, "Error\n");
        return 0;
    }
    fprintf(stderr, "OK\n");

    return (idx > 0) ? 1 : 0;
}

#else
int read_serial_port(char *buffer, size_t max_len) {
    int serial_port = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_SYNC);
    // int serial_port = open(SERIAL_PORT, O_RDWR);
    if (serial_port < 0) {
        fprintf(stderr, "Error %i from open: %s\n", errno, strerror(errno));
        return 0;
    }

    struct termios tty;
    memset(&tty, 0, sizeof tty);

    if (tcgetattr(serial_port, &tty) != 0) {
        fprintf(stderr, "Error %i from tcgetattr: %s\n", errno, strerror(errno));
        close(serial_port);
        return 0;
    }

    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    // tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    // tty.c_iflag &= ~IGNBRK;
    // tty.c_lflag = 0;
    // tty.c_oflag = 0;
    tty.c_cc[VMIN]  = 0;
    tty.c_cc[VTIME] = 0;
    //
    // tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    //
    // tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag |= CREAD | CLOCAL;
    // tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo

    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    // Set in/out baud rate to be 460800
    cfsetispeed(&tty, B460800);
    cfsetospeed(&tty, B460800);

    // same but 1 line
    // cfsetspeed(&tty, B460800);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        fprintf(stderr, "Error %i from tcsetattr: %s\n", errno, strerror(errno));
        close(serial_port);
        return 0;
    }

    // Flush any existing data in the buffer
    // tcflush(serial_port, TCIFLUSH);

    char c;
    int idx = 0;
    int found_newline = 0;

    // int n = read(serial_port, &buffer, sizeof(buffer));
    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be negative to signal an error.

    // Read until we encounter a newline character indicating start of data
    while (!found_newline && read(serial_port, &c, 1) == 1) {
        if (c == '^') {
            found_newline = 1;
        }
    }

    // Now read the actual data line
    if (found_newline) {
        idx = 0;
        while (idx < max_len && read(serial_port, &c, 1) == 1) {
            if (c == '|') {
                break;
            }
            if (c == '^') {
                idx = 0;
            }
            buffer[idx++] = c;
        }
        buffer[idx] = '\0'; // Null-terminate the string
    }

    close(serial_port);

    return (idx > 0) ? 1 : 0;
    // return (n > 0) ? 1 : 0;
}
#endif

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

    char buffer[MAX_DATA_LENGTH + 1] = {0};

    if (read_serial_port(buffer, MAX_DATA_LENGTH)) {
        char *tokens[MAX_TOKENS] = {0};
        int token_count = 0;
        char *token = strtok(buffer, ",");
        while (token && token_count < MAX_TOKENS) {
            tokens[token_count++] = strdup(token);
            token = strtok(NULL, ",");
        }

        if (token_count == 25) {
            // Temperature
            my_nicla_board.temperature.value = atof(tokens[0]);

            // Humidity
            my_nicla_board.humidity.value = atof(tokens[1]);

            // Pressure
            my_nicla_board.pressure.value = atof(tokens[2]);

            // Gas
            my_nicla_board.gas.value = atof(tokens[3]);

            // Magnetometer
            my_nicla_board.magnetometer.x.value = atoi(tokens[4]);
            my_nicla_board.magnetometer.y.value = atoi(tokens[5]);
            my_nicla_board.magnetometer.z.value = atoi(tokens[6]);

            // Gyroscope
            my_nicla_board.gyroscope.x.value = atoi(tokens[7]);
            my_nicla_board.gyroscope.y.value = atoi(tokens[8]);
            my_nicla_board.gyroscope.z.value = atoi(tokens[9]);

            // Accelerometer
            my_nicla_board.accelerometer.x.value = atoi(tokens[10]);
            my_nicla_board.accelerometer.y.value = atoi(tokens[11]);
            my_nicla_board.accelerometer.z.value = atoi(tokens[12]);

            // Quaternion
            my_nicla_board.quaternion.x.value = atof(tokens[13]);
            my_nicla_board.quaternion.y.value = atof(tokens[14]);
            my_nicla_board.quaternion.z.value = atof(tokens[15]);
            my_nicla_board.quaternion.w.value = atof(tokens[16]);

            // BSEC
            my_nicla_board.bsec.iaq.value = atoi(tokens[17]);
            my_nicla_board.bsec.iaq_s.value = atoi(tokens[18]);
            my_nicla_board.bsec.b_voc_eq.value = atof(tokens[19]);
            my_nicla_board.bsec.co2_eq.value = atoi(tokens[20]);
            my_nicla_board.bsec.accuracy.value = atoi(tokens[21]);
            my_nicla_board.bsec.comp_g.value = atoi(tokens[22]);
            my_nicla_board.bsec.comp_h.value = atof(tokens[23]);
            my_nicla_board.bsec.comp_t.value = atof(tokens[24]);

        }
    } else {
        fprintf(stderr, "No data received or incomplete data\n");
    }
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
    UA_Server_addRepeatedCallback(server, updateValuesFromNicla, NULL, 1000, NULL); /* call every 2 sec */

    UA_StatusCode retval = UA_Server_run(server, &running);
    UA_Server_delete(server);

    return retval == UA_STATUSCODE_GOOD ? EXIT_SUCCESS : EXIT_FAILURE;
}