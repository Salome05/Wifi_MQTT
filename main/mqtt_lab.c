/**
 * @file mqtt_lab.c
 * @brief Lab Session 2: Student Coding Exercises for MQTT & MQTTS (TLS)
 *
 * ============================================================================
 * STUDENT CODING TASKS IN THIS FILE:
 * ============================================================================
 * Search for `TODO (Task 1)` through `TODO (Task 5)` in this file:
 *
 *   - TASK 1 (`mqtt_event_handler`):
 *       Subscribe to `s_topic_cmd` (QoS 1), publish `"online"` (retained) to
 *       `s_topic_status` when `MQTT_EVENT_CONNECTED` fires, and safely copy
 *       incoming `MQTT_EVENT_DATA` buffers (which are NOT null-terminated!)
 *       before calling `mqtt_process_incoming_command()`.
 *
 *   - TASK 2 (`mqtt_app_start_plaintext`):
 *       Configure `esp_mqtt_client_config_t` for plaintext MQTT (`mqtt://`, Port 1883)
 *       including Last Will & Testament (LWT), initialize, register events, and start.
 *
 *   - TASK 3 (`mqtt_publish_telemetry`):
 *       Build a JSON telemetry payload with `cJSON`, serialize it, publish it to
 *       `s_topic_telemetry` with QoS 1, and free all allocated heap memory.
 *
 *   - TASK 4 (`mqtt_process_incoming_command`):
 *       Parse incoming JSON commands on `s_topic_cmd` (e.g., `{"action":"set_interval","value":3}`
 *       or `{"action":"ping"}`) and publish a JSON ACK response to `s_topic_status`.
 *
 *   - TASK 5 (`mqtt_app_start_tls`):
 *       Upgrade the connection to `mqtts://` (Port 8883) and attach your team's
 *       self-signed Root CA certificate (`LAB_CA_CERT_PEM` from `lab_certificates.h`)
 *       so mbedTLS verifies the broker's identity and encrypts all traffic!
 */

#include "mqtt_lab.h"
#include "lab_credentials.h"
#include "lab_certificates.h"
#include "wifi_sta.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp_log.h"
#include "esp_system.h"
#include "mqtt_client.h"
#include "cJSON.h"

static const char *TAG = "MQTT_LAB";

/* Global MQTT Client Handle & State */
static esp_mqtt_client_handle_t s_mqtt_client = NULL;
static bool     s_is_connected = false;
static uint32_t s_telemetry_interval_sec = 10; /* Default: 10 seconds */

/* Formatted Topic Buffers for this ESP32's Hardware MAC */
static char s_mac_clean[13]       = {0}; /* e.g., "2805A5749288" */
static char s_topic_telemetry[64] = {0}; /* e.g., "lab/devices/2805A5749288/telemetry" */
static char s_topic_cmd[64]       = {0}; /* e.g., "lab/devices/2805A5749288/cmd" */
static char s_topic_status[64]    = {0}; /* e.g., "lab/devices/2805A5749288/status" */

/* Forward declaration of command parser (Task 4) */
static void mqtt_process_incoming_command(const char *topic, const char *payload);

/* ============================================================================
 * HELPER: Initialize Device-Specific Topics from Hardware MAC
 * ============================================================================ */
void mqtt_lab_init_topics(void)
{
    char mac_colon[18] = {0};
    wifi_get_mac_str(mac_colon, sizeof(mac_colon));

    /* Strip colons from "28:05:A5:74:92:88" -> "2805A5749288" */
    int j = 0;
    for (int i = 0; mac_colon[i] != '\0' && j < (int)sizeof(s_mac_clean) - 1; i++) {
        if (mac_colon[i] != ':') {
            s_mac_clean[j++] = mac_colon[i];
        }
    }
    s_mac_clean[j] = '\0';

    snprintf(s_topic_telemetry, sizeof(s_topic_telemetry), "%s/%s/telemetry", LAB_TOPIC_PREFIX, s_mac_clean);
    snprintf(s_topic_cmd,       sizeof(s_topic_cmd),       "%s/%s/cmd",       LAB_TOPIC_PREFIX, s_mac_clean);
    snprintf(s_topic_status,    sizeof(s_topic_status),    "%s/%s/status",    LAB_TOPIC_PREFIX, s_mac_clean);

    ESP_LOGI(TAG, "Initialized Device MQTT Topics:");
    ESP_LOGI(TAG, "  [PUB] Telemetry : %s", s_topic_telemetry);
    ESP_LOGI(TAG, "  [SUB] Commands  : %s", s_topic_cmd);
    ESP_LOGI(TAG, "  [LWT] Status    : %s", s_topic_status);
}

/* ============================================================================
 * CODING TASK 4: Parse Incoming JSON Commands & Publish ACK
 * ============================================================================ */
/**
 * @brief Parse a JSON command received on `s_topic_cmd` and act on it.
 *
 * Expected JSON format sent by student from PC (`mosquitto_pub`):
 *   {"action": "set_interval", "value": 3}
 *   {"action": "ping"}
 *   {"action": "reboot"}
 */
static void mqtt_process_incoming_command(const char *topic, const char *payload)
{
    ESP_LOGI(TAG, "Processing command on [%s]: %s", topic, payload);

    /* ------------------------------------------------------------------------
     * TODO (Task 4): Parse `payload` using cJSON and execute the command!
     *
     * Steps to implement:
     *   1. Parse `payload` into `cJSON *root = cJSON_Parse(payload);`.
     *      If `root == NULL`, log an error and return immediately.
     *   2. Extract the `"action"` field using:
     *      `cJSON *action = cJSON_GetObjectItemCaseSensitive(root, "action");`
     *      Validate it with `cJSON_IsString(action) && action->valuestring != NULL`.
     *   3. Check which action was requested (`strcmp`):
     *      - If `"set_interval"`:
     *          Extract `"value"` (`cJSON_GetObjectItemCaseSensitive(root, "value")`).
     *          Verify `cJSON_IsNumber(val)` and `val->valueint >= 1 && val->valueint <= 60`.
     *          Update `s_telemetry_interval_sec = (uint32_t)val->valueint;`
     *          Publish an ACK string (e.g., `{"status":"interval_updated"}`) to `s_topic_status`.
     *      - If `"ping"`:
     *          Publish `{"status":"pong"}` to `s_topic_status` using `esp_mqtt_client_publish()`.
     *      - If `"reboot"`:
     *          Publish `{"status":"rebooting"}` and call `esp_restart()`.
     *   4. ALWAYS call `cJSON_Delete(root)` before returning!
     * ------------------------------------------------------------------------ */

    /* --- STUDENT CODE START (Task 4) --- */
    ESP_LOGW(TAG, "TODO (Task 4): Implement mqtt_process_incoming_command() in mqtt_lab.c!");
    /* --- STUDENT CODE END (Task 4) --- */
}

/* ============================================================================
 * CODING TASK 1: MQTT Event Handler (`MQTT_EVENT_CONNECTED` & `MQTT_EVENT_DATA`)
 * ============================================================================ */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base,
                               int32_t event_id, void *event_data)
{
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;
    esp_mqtt_client_handle_t client = event->client;
    (void)client; /* Suppress unused warning until student completes Task 1 */

    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED: Connected to Broker!");
            s_is_connected = true;

            /* ----------------------------------------------------------------
             * TODO (Task 1A): Subscribe to Command Topic & Publish Online Status
             *
             * Steps to implement:
             *   1. Subscribe to `s_topic_cmd` with QoS 1 using:
             *      `int msg_id = esp_mqtt_client_subscribe(client, s_topic_cmd, 1);`
             *      Log the `msg_id` returned.
             *   2. Publish a birth/status message `"{\"status\":\"online\"}"` to
             *      `s_topic_status` with length `0` (auto-strlen), QoS `1`, and
             *      `retain = 1` using `esp_mqtt_client_publish(client, topic, data, len, qos, retain)`.
             * ---------------------------------------------------------------- */

            /* --- STUDENT CODE START (Task 1A) --- */
            ESP_LOGW(TAG, "TODO (Task 1A): Subscribe to s_topic_cmd and publish online status!");
            /* --- STUDENT CODE END (Task 1A) --- */
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "MQTT_EVENT_DISCONNECTED: Broker connection lost.");
            s_is_connected = false;
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGD(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA received!");

            /* ----------------------------------------------------------------
             * TODO (Task 1B): Safely Extract Topic & Payload and Call Parser
             *
             * CRITICAL EMBEDDED C LESSON:
             *   `event->topic` (length `event->topic_len`) and
             *   `event->data`  (length `event->data_len`) are raw network pointers
             *   that are NOT null-terminated (`'\0'`)!
             *   Passing `event->data` directly to `printf("%s")` or `cJSON_Parse()`
             *   will read past the buffer and crash or corrupt memory!
             *
             * Steps to implement:
             *   1. Create local buffers:
             *        `char topic_buf[128] = {0};`
             *        `char data_buf[512]  = {0};`
             *   2. Copy at most `sizeof(topic_buf) - 1` bytes from `event->topic`
             *      using `memcpy()` or `snprintf(topic_buf, sizeof(topic_buf), "%.*s", event->topic_len, event->topic);`
             *   3. Copy at most `sizeof(data_buf) - 1` bytes from `event->data`
             *      and ensure `data_buf` is null-terminated.
             *   4. Call `mqtt_process_incoming_command(topic_buf, data_buf);`
             * ---------------------------------------------------------------- */

            /* --- STUDENT CODE START (Task 1B) --- */
            ESP_LOGW(TAG, "TODO (Task 1B): Copy event->topic and event->data safely and call mqtt_process_incoming_command()!");
            (void)mqtt_process_incoming_command;
            /* --- STUDENT CODE END (Task 1B) --- */
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                ESP_LOGE(TAG, "  Last esp-tls error code   : 0x%x", event->error_handle->esp_tls_last_esp_err);
                ESP_LOGE(TAG, "  Last tls stack error      : 0x%x", event->error_handle->esp_tls_stack_err);
                ESP_LOGE(TAG, "  Last cert verify flags    : 0x%x", event->error_handle->esp_tls_cert_verify_flags);
            }
            break;

        default:
            break;
    }
}

/* ============================================================================
 * CODING TASK 2: Start Plaintext MQTT Client (`mqtt://`, Port 1883 + LWT)
 * ============================================================================ */
esp_err_t mqtt_app_start_plaintext(void)
{
    mqtt_lab_init_topics();

    ESP_LOGI(TAG, "Starting Plaintext MQTT Client -> %s", LAB_MQTT_BROKER_PLAIN_URI);

    /* ------------------------------------------------------------------------
     * TODO (Task 2): Configure and Start the Plaintext MQTT Client with LWT
     *
     * Steps to implement:
     *   1. Declare and zero-initialize `esp_mqtt_client_config_t mqtt_cfg = {0};`
     *   2. Set `.broker.address.uri = LAB_MQTT_BROKER_PLAIN_URI;`
     *   3. Configure Last Will and Testament (LWT) inside `mqtt_cfg.session.last_will`:
     *        - `.topic  = s_topic_status`
     *        - `.msg    = "{\"status\":\"offline_unexpected\"}"`
     *        - `.msg_len = 0` (0 tells esp-mqtt to use strlen automatically)
     *        - `.qos    = 1`
     *        - `.retain = 1`
     *   4. Initialize the client:
     *        `s_mqtt_client = esp_mqtt_client_init(&mqtt_cfg);`
     *   5. Register the event handler:
     *        `esp_mqtt_client_register_event(s_mqtt_client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);`
     *   6. Start the client:
     *        `return esp_mqtt_client_start(s_mqtt_client);`
     * ------------------------------------------------------------------------ */

    /* --- STUDENT CODE START (Task 2) --- */
    (void)mqtt_event_handler;
    ESP_LOGW(TAG, "TODO (Task 2): Implement mqtt_app_start_plaintext() in mqtt_lab.c!");
    return ESP_ERR_NOT_SUPPORTED;
    /* --- STUDENT CODE END (Task 2) --- */
}

/* ============================================================================
 * CODING TASK 3: Build & Publish JSON Telemetry Payload
 * ============================================================================ */
esp_err_t mqtt_publish_telemetry(int rssi_dbm, uint32_t uptime_sec, uint32_t free_heap)
{
    if (!s_is_connected || s_mqtt_client == NULL) {
        ESP_LOGW(TAG, "Cannot publish telemetry: MQTT client not connected yet.");
        return ESP_FAIL;
    }

    /* ------------------------------------------------------------------------
     * TODO (Task 3): Serialize Telemetry with cJSON and Publish to Broker
     *
     * Target JSON structure:
     *   {
     *     "student_id": "team_01_alice_bob",
     *     "mac": "2805A5749288",
     *     "rssi_dbm": -48,
     *     "uptime_sec": 120,
     *     "free_heap": 218400
     *   }
     *
     * Steps to implement:
     *   1. Create a cJSON object: `cJSON *root = cJSON_CreateObject();`
     *   2. Add the 5 fields above using `cJSON_AddStringToObject()` and
     *      `cJSON_AddNumberToObject()`.
     *   3. Serialize to an unformatted string:
     *      `char *json_str = cJSON_PrintUnformatted(root);`
     *   4. Delete the cJSON tree immediately (`cJSON_Delete(root);`).
     *   5. Publish `json_str` to `s_topic_telemetry` with QoS `1` and `retain = 0`:
     *      `int msg_id = esp_mqtt_client_publish(s_mqtt_client, s_topic_telemetry, json_str, 0, 1, 0);`
     *   6. Free `json_str` (`free(json_str);`) so you don't leak heap every 10 seconds!
     *   7. Return `ESP_OK` if `msg_id >= 0`, else `ESP_FAIL`.
     * ------------------------------------------------------------------------ */

    /* --- STUDENT CODE START (Task 3) --- */
    (void)rssi_dbm;
    (void)uptime_sec;
    (void)free_heap;
    ESP_LOGW(TAG, "TODO (Task 3): Implement mqtt_publish_telemetry() in mqtt_lab.c!");
    return ESP_ERR_NOT_SUPPORTED;
    /* --- STUDENT CODE END (Task 3) --- */
}

/* ============================================================================
 * CODING TASK 5: Upgrade to MQTTS (`mqtts://`, Port 8883 + Self-Signed Root CA)
 * ============================================================================ */
esp_err_t mqtt_app_start_tls(void)
{
    mqtt_lab_init_topics();

    ESP_LOGI(TAG, "Starting Secure MQTTS (TLS) Client -> %s", LAB_MQTT_BROKER_TLS_URI);

    /* ------------------------------------------------------------------------
     * TODO (Task 5): Configure and Start the MQTTS Client with Root CA Verification
     *
     * Steps to implement:
     *   1. First, make sure you pasted your team's OpenSSL `ca.crt` into
     *      `LAB_CA_CERT_PEM` inside `main/lab_certificates.h`!
     *   2. Declare `esp_mqtt_client_config_t mqtt_cfg = {0};`
     *   3. Set `.broker.address.uri = LAB_MQTT_BROKER_TLS_URI;` (uses `mqtts://` port 8883).
     *   4. Attach your Root CA public certificate for server verification:
     *        `mqtt_cfg.broker.verification.certificate = LAB_CA_CERT_PEM;`
     *   5. Configure the same Last Will and Testament (LWT) on `s_topic_status` as in Task 2.
     *   6. (Optional Bonus - Mutual TLS / mTLS):
     *        If testing mTLS, also set:
     *        `mqtt_cfg.credentials.authentication.certificate = LAB_CLIENT_CERT_PEM;`
     *        `mqtt_cfg.credentials.authentication.key         = LAB_CLIENT_KEY_PEM;`
     *   7. Initialize `s_mqtt_client`, register `mqtt_event_handler`, and start the client!
     * ------------------------------------------------------------------------ */

    /* --- STUDENT CODE START (Task 5) --- */
    (void)LAB_CA_CERT_PEM;
    (void)LAB_CLIENT_CERT_PEM;
    (void)LAB_CLIENT_KEY_PEM;
    ESP_LOGW(TAG, "TODO (Task 5): Implement mqtt_app_start_tls() in mqtt_lab.c!");
    return ESP_ERR_NOT_SUPPORTED;
    /* --- STUDENT CODE END (Task 5) --- */
}

bool mqtt_lab_is_connected(void)
{
    return s_is_connected;
}

uint32_t mqtt_lab_get_interval_sec(void)
{
    return s_telemetry_interval_sec;
}
