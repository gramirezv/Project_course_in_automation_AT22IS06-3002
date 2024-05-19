#include "Nicla_System.h"
#include "Arduino_BHY2.h"

Sensor temperature(SENSOR_ID_TEMP);
Sensor humidity(SENSOR_ID_HUM);
Sensor pressure(SENSOR_ID_BARO);
Sensor gas(SENSOR_ID_GAS);

SensorXYZ gyroscope(SENSOR_ID_GYRO);
SensorXYZ accelerometer(SENSOR_ID_ACC);
SensorXYZ magnetometer(SENSOR_ID_MAG);

SensorQuaternion quaternion(SENSOR_ID_RV);

SensorBSEC bsec(SENSOR_ID_BSEC);


unsigned long previousMillis = 0;  // will store last time the sensor was updated
const long sampling_rate = 1000;   //100ms = 10Hz sampling rate

void setup() {
  Serial.begin(460800);
  while (!Serial)
    ;

  BHY2.begin();

  temperature.begin();
  humidity.begin();
  pressure.begin();
  gas.begin();

  gyroscope.begin();
  accelerometer.begin();
  magnetometer.begin();

  quaternion.begin();
  
  bsec.begin();


}

void loop() {
  static auto lastCheck = millis();

  // Update function should be continuously polled
  BHY2.update();

  // Check sensor values according the selected sampling rate
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= sampling_rate) {
    previousMillis = currentMillis;
    Serial.println(
        "^" +
        String(temperature.value())   + "," +
        String(humidity.value())      + "," +
        String(pressure.value())      + "," +
        String(gas.value())           + "," + 
        String(magnetometer.x())  + "," + String(magnetometer.y())  + "," + String(magnetometer.z())  + "," +
        String(gyroscope.x())     + "," + String(gyroscope.y())     + "," + String(gyroscope.z())     + "," +
        String(accelerometer.x()) + "," + String(accelerometer.y()) + "," + String(accelerometer.z()) + "," +
        String(quaternion.x())    + "," + String(quaternion.y())    + "," + String(quaternion.z())    + "," + String(quaternion.w()) + "," +
        String(bsec.iaq())        + "," + String(bsec.iaq_s())      + "," + String(bsec.b_voc_eq())   + "," + String(bsec.co2_eq())  + "," + 
        String(bsec.accuracy())   + "," + String(bsec.comp_g())     + "," + String(bsec.comp_h())     + "," + String(bsec.comp_t()) + "|");
  }
}