| Problem Type                                     | Graph Type               | Edge Weights | Goal                          | Recommended Algorithm         | Time Complexity            |
|--------------------------------------------------|--------------------------|--------------|-------------------------------|-------------------------------|----------------------------|
| Maximum matching                                 | Bipartite                | No           | Max number of matchings       | Hopcroft-Karp                 | O(√V · E)                  |
| Maximum matching (as max flow)                   | Bipartite or Directed    | No           | Max number of matchings       | Dinic                         | O(E · √V) or O(V² · E)     |
| Maximum matching                                 | General (non-bipartite)  | No           | Max number of matchings       | Blossom (Edmonds' Algorithm)  | O(V³)                      |
| Minimum cost matching                            | Bipartite                | Yes          | Min total weight              | Hungarian (Kuhn–Munkres)      | O(V³)                      |
| Maximum weight matching                          | Bipartite                | Yes          | Max total weight              | Hungarian (Kuhn–Munkres)      | O(V³)                      |
| General matching with augmenting paths           | General (non-bipartite)  | No           | Max matching                  | Blossom                       | O(V³)                      |
| Assignment problem (e.g. task to worker)         | Bipartite (n = m)        | Yes          | Optimal assignment            | Hungarian                     | O(V³)                      |
| Online matching or dynamic graph                 | Bipartite                | Varies       | Real-time updates             | Greedy or Incremental Method  | O(1)–O(E) per update       |
| Edge-disjoint paths / max # of s→t paths         | Directed Bipartite       | No           | Max number of disjoint paths  | Dinic / Ford-Fulkerson        | O(V² · E) or O(E · √V)     |
