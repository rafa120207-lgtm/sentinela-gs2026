# SENTINELA — Sistema Espacial de Previsão Climática

Global Solution 2026.1 — FIAP · Rafael + Charles

Plataforma que combina dados de satélites, sensores IoT e modelos de IA
para prever eventos climáticos extremos com antecedência.
Inspirado nas enchentes do RS 2024.

## Fluxo de dados

sensor esp32 (D1) → s3 + lambda (D6) → rds mysql (D3)
→ análise estatística (D8) → ml clássico (D5) → mlp (D7)
→ app + alertas (D4) · cybersec cobre tudo (D2)

## Disciplinas

| Pasta        | Disciplina                          | Responsável |
|--------------|-------------------------------------|-------------|
| d1-sensores/ | D1 · AI Computer Systems & Sensors  | Rafael      |
| d5-ml/       | D5 · Machine Learning & Modelling   | Rafael      |
| d7-neural/   | D7 · Redes Neurais e Deep Learning  | Rafael      |
| d2-cybersec/ | D2 · Cognitive Cybersecurity        | Rafael      |
| d3-banco/    | D3 · Cognitive Data Science         | Charles     |
| d8-stats/    | D8 · Statistical Computing          | Charles     |
| d6-aws/      | D6 · Plataformas Cognitivas AWS     | Charles     |
| d4-app/      | D4 · Computational Thinking Python  | Charles     |

## Dataset principal

INMET histórico · Porto Alegre · 2014–2025
Fonte: portal.inmet.gov.br/dadoshistoricos