// ============================================================
// SENTINELA · D1 — ESP32 · Estação Meteorológica
// timestamp_leitura + velocidade_vento_ms + pressao via pot
// ============================================================
// Componentes no Wokwi (todos sensores reais no campo,
// simulados no protótipo — o pipeline de dados é idêntico):
//   - DHT22            -> temperatura_c, umidade_pct  (GPIO15)
//   - Potenciometro A  -> precipitacao_mm             (GPIO34)
//   - Potenciometro B  -> velocidade_vento_ms         (GPIO35)
//   - Potenciometro C  -> pressao_hpa                 (GPIO32)
//   - LED vermelho     -> alerta                      (GPIO2)
//   - Buzzer           -> alerta                      (GPIO4)
//
// Nota: o BMP280 nao simula I2C no Wokwi. Em campo seria
// um BMP280 real; no prototipo simulamos a pressao com um
// potenciometro mapeado pra faixa realista (980-1030 hPa).
// ============================================================

#include <DHT.h>
#include <Arduino.h>

// -- Pinos --------------------------------------------------
#define DHT_PIN        15
#define DHT_TYPE       DHT22
#define PLUV_PIN       34    // potenciometro A -> chuva
#define VENTO_PIN      35    // potenciometro B -> vento
#define PRESSAO_PIN    32    // potenciometro C -> pressao
#define LED_PIN        2
#define BUZZER_PIN     4

// -- Thresholds de alerta -----------------------------------
#define CHUVA_LIMITE   30.0  // mm/h
#define UMID_LIMITE    90.0  // %
#define VENTO_LIMITE   20.0  // m/s

// -- Intervalo de leitura -----------------------------------
#define INTERVALO_MS   5000  // 5 segundos

DHT dht(DHT_PIN, DHT_TYPE);

unsigned long leituraNum = 0;

// -- Formata timestamp incrementando segundos ---------------
String timestampAtual() {
  unsigned long segundos = leituraNum * (INTERVALO_MS / 1000);
  unsigned long hh = (18 + segundos / 3600) % 24;
  unsigned long mm = (segundos % 3600) / 60;
  unsigned long ss = segundos % 60;
  char buf[25];
  sprintf(buf, "2026-06-03T%02lu:%02lu:%02lu", hh, mm, ss);
  return String(buf);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN,    OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  dht.begin();

  // Cabecalho CSV — nomes batem com a tabela LEITURA do D3
  Serial.println("timestamp_leitura,temperatura_c,umidade_pct,"
                 "pressao_hpa,precipitacao_mm,velocidade_vento_ms");
}

void loop() {
  // -- Ler sensores -----------------------------------------
  float temperatura_c = dht.readTemperature();
  float umidade_pct   = dht.readHumidity();

  // Potenciometro A: 0-4095 -> 0.0-100.0 mm/h (chuva)
  float precipitacao_mm = map(analogRead(PLUV_PIN), 0, 4095, 0, 1000) / 10.0;

  // Potenciometro B: 0-4095 -> 0.0-30.0 m/s (vento)
  float velocidade_vento_ms = map(analogRead(VENTO_PIN), 0, 4095, 0, 300) / 10.0;

  // Potenciometro C: 0-4095 -> 980.0-1030.0 hPa (pressao)
  float pressao_hpa = 980.0 + (analogRead(PRESSAO_PIN) / 4095.0) * 50.0;

  // -- Validar leituras DHT ---------------------------------
  if (isnan(temperatura_c) || isnan(umidade_pct)) {
    Serial.println("ERRO: falha na leitura DHT22");
    delay(INTERVALO_MS);
    return;
  }

  // -- Montar e imprimir linha CSV --------------------------
  Serial.print(timestampAtual());          Serial.print(",");
  Serial.print(temperatura_c,  2);          Serial.print(",");
  Serial.print(umidade_pct,    2);          Serial.print(",");
  Serial.print(pressao_hpa,    2);          Serial.print(",");
  Serial.print(precipitacao_mm, 2);         Serial.print(",");
  Serial.println(velocidade_vento_ms, 2);

  // -- Logica de alerta -------------------------------------
  bool alerta = (precipitacao_mm     > CHUVA_LIMITE) ||
                (umidade_pct         > UMID_LIMITE)  ||
                (velocidade_vento_ms > VENTO_LIMITE);

  digitalWrite(LED_PIN, alerta ? HIGH : LOW);

  if (alerta) {
    tone(BUZZER_PIN, 2000);   // bip de 2 kHz
    Serial.println("# ALERTA ATIVO -> LED + buzzer ligados");
  } else {
    noTone(BUZZER_PIN);       // silencia
  }

  leituraNum++;
  delay(INTERVALO_MS);
}
