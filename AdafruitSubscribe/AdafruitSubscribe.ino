#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// --- Credenciales Wi-Fi y Adafruit IO ---
#define WLAN_SSID       ""
#define WLAN_PASS       ""

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    ""
//#define AIO_KEY         ""

#define LED1_PIN 2   // GPIO 2 (LED integrado o externo)
#define LED2_PIN 4   // GPIO 4 (Segundo LED)
#define LED3_PIN 23
#define LED4_PIN 22
#define LED5_PIN 21

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Una sola suscripción para ambos LEDs: el feed del botón único
Adafruit_MQTT_Subscribe sub_feed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led-chevere");

void MQTT_connect();

// Callback que controla ambos pines juntos
void feedCallback(char *data, uint16_t len) {
  Serial.print(F("Comando recibido: "));
  Serial.println(data);

  if (strcmp(data, "ON") == 0 || strcmp(data, "1") == 0) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    digitalWrite(LED4_PIN, HIGH);
    digitalWrite(LED5_PIN, HIGH);
    Serial.println(F("-> AMBOS LEDs ENCENDIDOS"));
  } 
  else if (strcmp(data, "OFF") == 0 || strcmp(data, "0") == 0) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(LED4_PIN, LOW);
    digitalWrite(LED5_PIN, LOW);
    Serial.println(F("-> AMBOS LEDs APAGADOS"));
  }
}

void setup() {
  Serial.begin(115200);
  delay(10);

  // Configurar ambos pines como salida
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(LED5_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);
  digitalWrite(LED5_PIN, LOW);

  Serial.println(F("\nConectando a Wi-Fi..."));
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(F("\nWi-Fi conectado."));

  // Asociar la función al feed
  sub_feed.setCallback(feedCallback);
  mqtt.subscribe(&sub_feed);
}

void loop() {
  MQTT_connect();

  // Escucha mensajes entrantes
  mqtt.processPackets(5000);

  if (!mqtt.ping()) {
    mqtt.disconnect();
  }
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print(F("Conectando a MQTT... "));

  while ((ret = mqtt.connect()) != 0) { 
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println(F("Reintentando conexión MQTT en 5 segundos..."));
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println(F("¡MQTT Conectado!"));
}