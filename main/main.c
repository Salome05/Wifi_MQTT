/**
 * @file main.c
 * @brief Hands-On Lab Session 2: ESP32 MQTT & MQTT over TLS (PKI / X.509)
 *
 * ============================================================================
 * LAB OVERVIEW (3-HOUR SESSION 2):
 * ============================================================================
 *  - MODULE 1: MQTT Protocol Architecture & Local PC Mosquitto Broker Setup
 *  - MODULE 2: Plaintext MQTT Client (`mqtt://`), LWT, JSON Telemetry & Commands
 *  - MODULE 3: PKI Cryptography (Private/Public Keys, Root CA, CSR, SAN IP) with OpenSSL
 *  - MODULE 4: Secure MQTT over TLS (`mqtts://`, Port 8883) & Certificate Verification
 *
 * STUDENT INSTRUCTIONS:
 *  1. Edit `main/lab_credentials.h` with your Wi-Fi SSID/Password and your PC's IP.
 *  2. Edit `main/lab_certificates.h` in Module 3/4 with your OpenSSL `ca.crt`.
 *  3. Complete `TODO (Task 1)` through `TODO (Task 5)` inside `main/mqtt_lab.c`.
 *  4. Toggle `LAB_ENABLE_MQTT_PLAINTEXT` vs. `LAB_ENABLE_MQTT_TLS` below.
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_timer.h"

#include "lab_credentials.h"
#include "wifi_sta.h"
#include "http_rest.h"
#include "mqtt_lab.h"

static const char *TAG = "LAB_SESSION2";

/* ============================================================================
 * PROGRESSIVE LAB STEP TOGGLES (Set to 1 to enable, 0 to disable)
 * ============================================================================
 * NOTE: Enable EITHER Plaintext (Module 2) OR TLS (Module 4) at one time!
 */
#define LAB_ENABLE_HTTP_ENROLL      0   /* Optional: Re-run Session 1 FastAPI enrollment */
#define LAB_ENABLE_MQTT_PLAINTEXT   1   /* Module 2: Connect via mqtt://<PC_IP>:1883     */
#define LAB_ENABLE_MQTT_TLS         0   /* Module 4: Connect via mqtts://<PC_IP>:8883    */

void app_main(void)
{
    ESP_LOGI(TAG, "======================================================");
    ESP_LOGI(TAG, " ESP32 Hands-On Lab: MQTT & MQTTS over TLS (Session 2)");
    ESP_LOGI(TAG, "======================================================");

    /* ------------------------------------------------------------------------
     * STEP 1: Connect to Classroom Wi-Fi (Credentials in lab_credentials.h)
     * ------------------------------------------------------------------------ */
    ESP_LOGI(TAG, "[STEP 1] Connecting to Wi-Fi SSID: %s", LAB_WIFI_SSID);
    if (wifi_init_sta(LAB_WIFI_SSID, LAB_WIFI_PASS) != ESP_OK) {
        ESP_LOGE(TAG, "Wi-Fi connection failed! Check main/lab_credentials.h.");
        return;
    }

    char mac_str[18] = {0};
    wifi_get_mac_str(mac_str, sizeof(mac_str));
    ESP_LOGI(TAG, "Device MAC: %s | RSSI: %d dBm", mac_str, wifi_get_rssi());

#if LAB_ENABLE_HTTP_ENROLL
    /* Optional bridge from Session 1: Retrieve assigned topic from FastAPI */
    ESP_LOGI(TAG, "[OPTIONAL] Enrolling device with FastAPI backend...");
    http_rest_post_enrollment(FASTAPI_SERVER_URL, LAB_STUDENT_ID);
#endif

#if LAB_ENABLE_MQTT_PLAINTEXT && LAB_ENABLE_MQTT_TLS
    #error "Please enable ONLY ONE of LAB_ENABLE_MQTT_PLAINTEXT or LAB_ENABLE_MQTT_TLS!"
#endif

#if LAB_ENABLE_MQTT_PLAINTEXT
    /* ------------------------------------------------------------------------
     * MODULE 2: Start Plaintext MQTT Client (Port 1883)
     * ------------------------------------------------------------------------ */
    ESP_LOGI(TAG, "[MODULE 2] Starting Plaintext MQTT Client...");
    mqtt_app_start_plaintext();
#elif LAB_ENABLE_MQTT_TLS
    /* ------------------------------------------------------------------------
     * MODULE 4: Start Secure MQTTS Client over TLS (Port 8883)
     * ------------------------------------------------------------------------ */
    ESP_LOGI(TAG, "[MODULE 4] Starting Secure MQTTS (TLS) Client...");
    mqtt_app_start_tls();
#endif

    /* ------------------------------------------------------------------------
     * TELEMETRY LOOP: Periodically publish JSON telemetry (Coding Task 3)
     * ------------------------------------------------------------------------ */
    while (1) {
        uint32_t interval_sec = mqtt_lab_get_interval_sec();
        vTaskDelay(pdMS_TO_TICKS(interval_sec * 1000));

        uint32_t uptime_sec = (uint32_t)(esp_timer_get_time() / 1000000ULL);
        uint32_t free_heap  = esp_get_free_heap_size();
        int      rssi       = wifi_get_rssi();

        if (mqtt_lab_is_connected()) {
            mqtt_publish_telemetry(rssi, uptime_sec, free_heap);
        } else {
            ESP_LOGW(TAG, "[Waiting for MQTT] RSSI: %d dBm | Uptime: %lu s | Heap: %lu B",
                     rssi, (unsigned long)uptime_sec, (unsigned long)free_heap);
        }
    }
}
