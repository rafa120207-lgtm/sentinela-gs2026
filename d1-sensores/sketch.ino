#include "DHT.h"

#define DHTPIN      15
#define DHTTYPE     DHT22
#define PRES_PIN    35
#define CHUVA_PIN   34
#define LED_PIN     2
#define BUZZER_PIN  4
#define UMID_MAX    90.0f
#define PRES_MIN    980.0f
#define PRES_MAX    1030.0f

DHT dht(DHTPIN, DHTTYPE);

bool verificarAlerta(float chuva_mmh, float umidade) {
  bool alerta = (chuva_mmh > 30.0f) || (umidade > UMID_MAX);
  digitalWrite(LED_PIN, alerta ? HIGH : LOW);
  if (alerta) { tone(BUZZER_PIN, 1000, 300); }
  else        { noTone(BUZZER_PIN); }
  return alerta;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN,    OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN,    LOW);
  digitalWrite(BUZZER_PIN, LOW);
  dht.begin();
  Serial.println("SENTINELA - D1 iniciado - todos os sensores OK");
  Serial.println("timestamp_s,temperatura_c,umidade_pct,pressao_hpa,chuva_mm,status");
}

void loop() {
  float temperatura = dht.readTemperature();
  float umidade     = dht.readHumidity();
  int   rawPres     = analogRead(PRES_PIN);
  float pressao     = PRES_MIN + (rawPres / 4095.0f) * (PRES_MAX - PRES_MIN);
  int   rawChuva    = analogRead(CHUVA_PIN);
  float chuva       = (rawChuva / 4095.0f) * 100.0f;

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("[ERRO] Falha na leitura do DHT22");
    delay(2000);
    return;
  }

  bool alerta = verificarAlerta(chuva, umidade);

  unsigned long ts = millis() / 1000;
  Serial.print(ts);             Serial.print(",");
  Serial.print(temperatura, 1); Serial.print(",");
  Serial.print(umidade,     1); Serial.print(",");
  Serial.print(pressao,     1); Serial.print(",");
  Serial.print(chuva,       1); Serial.print(",");
  Serial.println(alerta ? "ALERTA" : "OK");

  delay(5000);
}