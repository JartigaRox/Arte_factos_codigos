// ==========================================================
// Proyecto 3: Luz nocturna automática con temporizador
// Mismo circuito: Trig=5, Echo=18, LEDs=21, 22, 23
// ==========================================================
const int pinTrig = 5;
const int pinEcho = 18;

const int led1 = 21;
const int led2 = 22;
const int led3 = 23;

// Configuración de umbral y tiempo
const int distanciaDeteccion = 40;          // Distancia de detección en cm
const unsigned long tiempoEncendido = 8000; // Tiempo que permanece encendido tras el último paso (8 segundos)

unsigned long ultimoMovimiento = 0;
bool lucesEncendidas = false;

float medirDistancia() {
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);

  long duracion = pulseIn(pinEcho, HIGH, 30000);
  if (duracion == 0) return 999.0;
  return duracion / 58.2;
}

void encenderLeds() {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
}

void apagarLeds() {
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  apagarLeds();
  Serial.println("Luz nocturna lista. Esperando presencia...");
}

void loop() {
  float distancia = medirDistancia();

  // Si detecta a alguien dentro del rango
  if (distancia > 0 && distancia <= distanciaDeteccion) {
    ultimoMovimiento = millis(); // Reinicia el temporizador al último instante detectado

    if (!lucesEncendidas) {
      encenderLeds();
      lucesEncendidas = true;
      Serial.println("Movimiento detectado: Luces ENCENDIDAS");
    }
  }

  // Si las luces están encendidas, revisa si ya expiró el tiempo de espera
  if (lucesEncendidas) {
    if (millis() - ultimoMovimiento >= tiempoEncendido) {
      apagarLeds();
      lucesEncendidas = false;
      Serial.println("Sin movimiento: Tiempo agotado -> Luces APAGADAS");
    }
  }

  delay(60); // Pequeño intervalo entre lecturas
}