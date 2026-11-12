/**
 * @file lab_certificates.h
 * @brief Self-Signed Root CA & Mutual TLS (mTLS) X.509 Certificates Header
 *
 * ============================================================================
 * WHY DO WE NEED THIS FILE?
 * ============================================================================
 * When the ESP32 connects to your PC's Mosquitto broker over `mqtts://` (Port 8883),
 * mbedTLS performs a TLS handshake:
 *   1. Your PC's Mosquitto broker sends its `server.crt` (public certificate).
 *   2. How does the ESP32 know `server.crt` isn't from an attacker spoofing your IP?
 *   3. The ESP32 uses `LAB_CA_CERT_PEM` below (your self-signed Root CA public
 *      certificate) to cryptographically verify the digital signature on `server.crt`
 *      AND checks that the broker's IP matches the certificate's `subjectAltName`!
 *
 * SECURITY RULE:
 *   - NEVER put `ca.key` or `server.key` (Private Keys) on the ESP32!
 *   - `LAB_CA_CERT_PEM` is the PUBLIC Root CA certificate (`ca.crt`).
 *   - Only if you do the Bonus Challenge (Mutual TLS / mTLS) will you paste a
 *     device-specific `client.crt` and `client.key` in the Bonus section below.
 *
 * STUDENT INSTRUCTIONS (Module 3 & 4):
 *   After running OpenSSL to generate your team's `ca.crt`, copy the entire
 *   contents of `ca.crt` (including `-----BEGIN CERTIFICATE-----` and
 *   `-----END CERTIFICATE-----`) and paste it into `LAB_CA_CERT_PEM` below.
 */

#ifndef LAB_CERTIFICATES_H
#define LAB_CERTIFICATES_H

/* ============================================================================
 * 1. SERVER-SIDE TLS: Team Root CA Public Certificate (`ca.crt`)
 * ============================================================================
 * TODO (Module 3/4): Replace the placeholder PEM below with the contents of
 *                    your team's generated `ca.crt` file.
 */
static const char LAB_CA_CERT_PEM[] =
"-----BEGIN CERTIFICATE-----\n"
"MIIDCTCCAfGgAwIBAgIUX9pplaceholderRootCACertificate00000000000000\n"
"MA0GCSqGSIb3DQEBCwUAMBkxFzAVBgNVBAMMDklvVC1MYWItUm9vdC1DQTAeFw0y\n"
"NjAxMDEwMDAwMDBaFw0zNjAxMDEwMDAwMDBaMBkxFzAVBgNVBAMMDklvVC1MYWIt\n"
"Um9vdC1DQTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALplaceholder\n"
"REPLACE_THIS_WITH_YOUR_TEAM_OPENSSL_GENERATED_CA_CRT_CONTENTS_HERE\n"
"-----END CERTIFICATE-----\n";

/* ============================================================================
 * 2. BONUS CHALLENGE (mTLS): ESP32 Client Certificate & Client Private Key
 * ============================================================================
 * Only used if `require_certificate true` is enabled in `mosquitto_tls.conf`.
 */
static const char LAB_CLIENT_CERT_PEM[] =
"-----BEGIN CERTIFICATE-----\n"
"PASTE_YOUR_OPTIONAL_MTLS_CLIENT_CRT_HERE\n"
"-----END CERTIFICATE-----\n";

static const char LAB_CLIENT_KEY_PEM[] =
"-----BEGIN PRIVATE KEY-----\n"
"PASTE_YOUR_OPTIONAL_MTLS_CLIENT_KEY_HERE\n"
"-----END PRIVATE KEY-----\n";

#endif /* LAB_CERTIFICATES_H */
