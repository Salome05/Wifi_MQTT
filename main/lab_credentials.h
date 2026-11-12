/**
 * @file lab_credentials.h
 * @brief Wi-Fi, REST API, and Local MQTT Broker Credentials Configuration
 *
 * ============================================================================
 * STUDENT INSTRUCTIONS:
 * ============================================================================
 * 1. Set your classroom 2.4 GHz Wi-Fi SSID and Password below.
 * 2. Set your `LAB_STUDENT_ID` (used for FastAPI enrollment & MQTT client ID).
 * 3. Replace `192.168.1.100` in `LAB_MQTT_BROKER_PLAIN_URI` and
 *    `LAB_MQTT_BROKER_TLS_URI` with the local IPv4 address of YOUR team's PC
 *    running the Mosquitto broker (`ip -4 addr` on Linux / `ipconfig` on Windows).
 */

#ifndef LAB_CREDENTIALS_H
#define LAB_CREDENTIALS_H

/* ----------------------------------------------------------------------------
 * 1. Wi-Fi Station Credentials (2.4 GHz Required for ESP32)
 * ---------------------------------------------------------------------------- */
#define LAB_WIFI_SSID               "YOUR_LAB_WIFI_SSID"
#define LAB_WIFI_PASS               "YOUR_LAB_WIFI_PASSWORD"

/* ----------------------------------------------------------------------------
 * 2. Student / Team Identification & REST Provisioning Endpoint
 * ---------------------------------------------------------------------------- */
#define LAB_STUDENT_ID              "team_01_alice_bob"
#define FASTAPI_SERVER_URL          "http://eniacelectronics.cloud:8000/api/v1/enroll"

/* ----------------------------------------------------------------------------
 * 3. Local Team Mosquitto Broker Endpoints & Credentials
 * ----------------------------------------------------------------------------
 * IMPORTANT: In Module 3 & 4, the IP address in `LAB_MQTT_BROKER_TLS_URI` MUST
 * exactly match the `subjectAltName = IP:x.x.x.x` baked into your `server.crt`!
 */
#define LAB_MQTT_BROKER_PLAIN_URI   "mqtt://192.168.1.100:1883"
#define LAB_MQTT_BROKER_TLS_URI     "mqtts://192.168.1.100:8883"

/* Optional MQTT Username / Password (if password_file is enabled in Mosquitto) */
#define LAB_MQTT_USERNAME           "iot_student"
#define LAB_MQTT_PASSWORD           "iot_secret_pass"

/* ----------------------------------------------------------------------------
 * 4. MQTT Topic Hierarchy Templates (Replaced with device MAC at runtime)
 * ----------------------------------------------------------------------------
 *   Telemetry (ESP32 -> Broker) : lab/devices/<MAC>/telemetry
 *   Commands  (Broker -> ESP32) : lab/devices/<MAC>/cmd
 *   Status/LWT(ESP32 -> Broker) : lab/devices/<MAC>/status
 */
#define LAB_TOPIC_PREFIX            "lab/devices"

#endif /* LAB_CREDENTIALS_H */
