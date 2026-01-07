# Performance Analysis Report: Matrix Inversion

## 1. Bottleneck: $O(n!)$ Complexity
The Flamegraph reveals `mat_inverse` consumes nearly all execution time. The use of Laplace expansion (cofactors) triggers millions of recursive `mat_determinant` calls for a $10 \times 10$ matrix, rendering all other operations negligible.

## 2. Call Stack & Memory Overhead
The vertical axis shows an extremely deep call stack ("stalactite" effect) due to recursion. Frequent `mat_mineur` calls trigger constant `mat_create` and `malloc` invocations. This results in jagged peaks where CPU cycles are wasted on memory management rather than computation.

## 3. OpenMP Strategy
Parallelizing `fill_random` proved ineffective as it targets $<1\%$ of execution time. Significant speedups require optimizing `mat_inverse`—either by parallelizing its loops or, ideally, switching to a parallelized Gaussian elimination.

## 4. Summary

| Observation | Root Cause | Action Plan |
| :--- | :--- | :--- |
| Wide `mat_inverse` bar | $O(n!)$ Laplace algorithm | Switch to Gauss-Jordan ($O(n^3)$). |
| Jagged `malloc` peaks | Excessive allocation | Use in-place calculations. |
| Inefficient OpenMP | Misplaced optimization | Parallelize `mat_inverse`. |

## Conclusion
Optimizing initialization was premature. Future iterations must prioritize a more efficient inversion algorithm combined with OpenMP to effectively leverage multi-core processing.
