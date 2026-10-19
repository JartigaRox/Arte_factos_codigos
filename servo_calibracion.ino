#include <ESP32Servo.h>

const int PIN_SERVO = 18;          // Cable naranja (señal) -> G18
const int PULSO_MIN = 500;         // µs correspondientes a 0°
const int PULSO_MAX = 2400;        // µs correspondientes a 180°
const unsigned long ESPERA = 2000; // ms entre posiciones

Servo servo;

void setup() {
  Serial.begin(115200);

  ESP32PWM::allocateTimer(0);
  servo.setPeriodHertz(50);                    // Los servos SG90 usan 50 Hz
  servo.attach(PIN_SERVO, PULSO_MIN, PULSO_MAX);

  Serial.println("Calibrando a 0°");
  servo.write(0);
  delay(ESPERA);

  Serial.println("Moviendo a 90°");
  servo.write(90);
  delay(ESPERA);

  Serial.println("Moviendo a 180°");
  servo.write(180);
  delay(ESPERA);

  Serial.println("Secuencia terminada");
}

void loop() {
  // Vacío: la secuencia se ejecuta una sola vez al iniciar
  Serial.println("Calibrando a 0°");
  servo.write(0);
  delay(ESPERA);

  Serial.println("Moviendo a 90°");
  servo.write(90);
  delay(ESPERA);

  Serial.println("Moviendo a 180°");
  servo.write(180);
  delay(ESPERA);

  Serial.println("Secuencia terminada");
}
