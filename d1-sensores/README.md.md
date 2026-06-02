# SENTINELA · D1 — Estação Meteorológica IoT

Sistema de monitoramento ambiental embarcado em ESP32, desenvolvido para o projeto SENTINELA (Global Solution 2026.1 — FIAP). Coleta dados climáticos em tempo real e dispara alertas locais quando detecta condições de risco.

## Sensores e atuadores

| Componente | Pino | Dado coletado |
|---|---|---|
| DHT22 | GPIO 15 | Temperatura (°C) e umidade (%) |
| Potenciômetro (simula BMP280) | GPIO 35 | Pressão atmosférica (980–1030 hPa) |
| Potenciômetro (simula pluviômetro) | GPIO 34 | Chuva (0–100 mm/h) |
| LED vermelho | GPIO 2 | Alerta visual |
| Buzzer | GPIO 4 | Alerta sonoro |

## Lógica de alerta

Alerta ativo se **chuva > 30 mm/h** ou **umidade > 90%** → LED acende + buzzer apita.

## Saída serial (CSV)

```
timestamp_s,temperatura_c,umidade_pct,pressao_hpa,chuva_mm,status
12,24.0,40.0,998.3,32.5,ALERTA
```

Formato compatível com a tabela `LEITURA` do banco (D3) e pipeline AWS (D6).

## Como rodar

1. Acesse o projeto: [https://wokwi.com/projects/465300118520840193](https://wokwi.com/projects/465300118520840193)
2. Clique em ▶ para iniciar a simulação
3. Abra o Serial Monitor (115200 baud)
4. Gire o potenciômetro de chuva (GPIO 34) acima de 50% para acionar o alerta

## Contexto SENTINELA

Este módulo é a **fonte de dados em tempo real** do sistema. As leituras saem via serial em CSV e alimentam o pipeline S3 → Lambda → RDS (D6), onde são armazenadas para análise estatística (D8) e modelos de ML (D5/D7).
