# Min-Cost Max-Flow (MCMF)

## Overview
Min-Cost Max-Flow algorithms find the cheapest way to route the maximum possible flow through a network.

## Successive Shortest Path (SSP)
**Concept:** Repeatedly find the cheapest augmenting path from Source to Sink and push as much flow as possible.
* **SPFA Variant:** Uses SPFA to handle negative edge costs directly. Fast in practice but vulnerable to worst-case $\mathcal{O}(V^2 E)$ if anti-SPFA graphs are present.
* **Dijkstra with Potentials:** Uses Bellman-Ford once to establish node potentials, then uses Dijkstra for all subsequent augmentations by adjusting edge weights to be non-negative.
* **Problem Signatures:** "Match $N$ items to $M$ targets with minimum cost", "Routing flow with penalties".
* **Complexity:** $\mathcal{O}(F \cdot E \log V)$ where $F$ is the maximum flow.
