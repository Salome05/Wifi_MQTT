/**
 * @file mqtt_lab.h
 * @brief Lab Session 2: MQTT & MQTT over TLS (MQTTS) Student Interface
 *
 * This module defines the API that `app_main()` calls to:
 *  1. Format device-specific MQTT topics based on the ESP32 hardware MAC.
 *  2. Start the MQTT client in Plaintext mode (`mqtt://`, Port 1883) with LWT.
 *  3. Start the MQTT client in TLS mode (`mqtts://`, Port 8883) with X.509 CA verification.
 *  4. Publish JSON telemetry payloads and process incoming JSON remote commands.
 */

#ifndef MQTT_LAB_H
#define MQTT_LAB_H

#include "esp_err.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize topic strings (`telemetry`, `cmd`, `status`) using the ESP32's MAC.
 *
 * Example generated topics for MAC `28:05:A5:74:92:88`:
 *   - Telemetry : `lab/devices/2805A5749288/telemetry`
 *   - Command   : `lab/devices/2805A5749288/cmd`
 *   - Status    : `lab/devices/2805A5749288/status`
 */
void mqtt_lab_init_topics(void);

/**
 * @brief Start the MQTT client over Plaintext TCP (`mqtt://<PC_IP>:1883`).
 *
 * Students implement Coding Task 2 inside this function:
 *  - Configure broker URI (`LAB_MQTT_BROKER_PLAIN_URI`)
 *  - Configure Last Will and Testament (LWT) on the status topic
 *  - Initialize client, register `mqtt_event_handler`, and start client
 *
 * @return esp_err_t ESP_OK on success.
 */
esp_err_t mqtt_app_start_plaintext(void);

/**
 * @brief Start the MQTT client over TLS (`mqtts://<PC_IP>:8883`) using `LAB_CA_CERT_PEM`.
 *
 * Students implement Coding Task 5 inside this function:
 *  - Configure broker URI (`LAB_MQTT_BROKER_TLS_URI`)
 *  - Attach `LAB_CA_CERT_PEM` from `lab_certificates.h` to `.broker.verification.certificate`
 *  - Configure Last Will and Testament (LWT)
 *  - (Bonus) Attach client certificate & private key for Mutual TLS (mTLS)
 *
 * @return esp_err_t ESP_OK on success.
 */
esp_err_t mqtt_app_start_tls(void);

/**
 * @brief Serialize device telemetry into JSON and publish to `lab/devices/<MAC>/telemetry`.
 *
 * Students implement Coding Task 3 inside this function.
 *
 * @param rssi_dbm    Current Wi-Fi RSSI in dBm.
 * @param uptime_sec  Device uptime in seconds.
 * @param free_heap   Available free heap memory in bytes.
 * @return esp_err_t  ESP_OK if published (msg_id >= 0), ESP_FAIL otherwise.
 */
esp_err_t mqtt_publish_telemetry(int rssi_dbm, uint32_t uptime_sec, uint32_t free_heap);

/**
 * @brief Return true if the MQTT client is currently connected to the broker.
 */
bool mqtt_lab_is_connected(void);

/**
 * @brief Get the current telemetry reporting interval in seconds (modifiable via MQTT command).
 */
uint32_t mqtt_lab_get_interval_sec(void);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_LAB_H */
