# D7 — Redes Neurais (MLP) · SENTINELA GS 2026.1

## Objetivo
MLP para classificação de eventos climáticos extremos (chuva > 30mm/24h).
Mesmo dataset e split do D5 — comparação direta entre ML clássico e rede neural.

## Arquitetura
Dense(64, relu) → Dropout(0.2) → Dense(32, relu) → Dense(1, sigmoid)

## Resultados (conjunto de teste)
| Modelo          | Recall (1) | F1-Score (1) |
|:----------------|:----------:|:------------:|
| Random Forest   | 0.1667     | 0.2857       |
| XGBoost tuned   | 0.1667     | 0.1818       |
| **MLP (D7)**    | **1.0000** | **0.9231**   |

## Arquivos
- `sentinela_d7_mlp_final.ipynb` — notebook completo
- `sentinela_mlp_d7.keras` — modelo treinado
- `sentinela_scaler_d7.pkl` — scaler (fit no treino)
- `sentinela_imputer_d7.pkl` — imputer (fit no treino)
