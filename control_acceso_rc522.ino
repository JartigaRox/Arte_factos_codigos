#include <SPI.h>
#include <MFRC522.h>

// ---------- Pines (SPI por defecto del ESP32) ----------
const int PIN_SCK  = 18;  // SCK  -> G18
const int PIN_MISO = 19;  // MISO -> G19
const int PIN_MOSI = 23;  // MOSI -> G23
const int PIN_SS   = 5;   // SDA  -> G5
const int PIN_RST  = 22;  // RST  -> G22
const int PIN_LED  = 2;   // LED integrado de la placa (si la tuya lo trae)

// ---------- UID del chip que tendrá ACCESO ----------
// Reemplaza estos bytes por el UID real que verás en el monitor serie.
byte UID_AUTORIZADO[] = {0x00, 0x00, 0x00, 0x00};
const byte LARGO_UID = sizeof(UID_AUTORIZADO);

MFRC522 lector(PIN_SS, PIN_RST);

// Compara el UID leído con el autorizado
bool esAutorizado(MFRC522::Uid &uid) {
  if (uid.size != LARGO_UID) return false;
  for (byte i = 0; i < uid.size; i++) {
    if (uid.uidByte[i] != UID_AUTORIZADO[i]) return false;
  }
  return true;
}

// Imprime el UID en formato 0xAA, 0xBB, ...
void imprimirUID(MFRC522::Uid &uid) {
  for (byte i = 0; i < uid.size; i++) {
    Serial.print("0x");
    if (uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(uid.uidByte[i], HEX);
    if (i < uid.size - 1) Serial.print(", ");
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_SS);
  lector.PCD_Init();
  delay(100);

  Serial.println("Lector RC522 listo");
  lector.PCD_DumpVersionToSerial();  // Si muestra 0x00 o 0xFF, revisa el cableado
  Serial.println("Acerca un chip al lector...");
}

void loop() {
  // Espera a que haya una tarjeta nueva cerca y se pueda leer
  if (!lector.PICC_IsNewCardPresent()) return;
  if (!lector.PICC_ReadCardSerial()) return;

  Serial.print("UID leido: ");
  imprimirUID(lector.uid);

  if (esAutorizado(lector.uid)) {
    Serial.println(">>> ACCESO CONCEDIDO");
    digitalWrite(PIN_LED, HIGH);
    delay(2000);
    digitalWrite(PIN_LED, LOW);
  } else {
    Serial.println(">>> ACCESO DENEGADO");
    for (int i = 0; i < 3; i++) {   // 3 parpadeos rápidos
      digitalWrite(PIN_LED, HIGH);
      delay(150);
      digitalWrite(PIN_LED, LOW);
      delay(150);
    }
  }

  lector.PICC_HaltA();       // Detiene la comunicación con el chip
  lector.PCD_StopCrypto1();  // Libera el lector para la siguiente lectura
  delay(500);
}
