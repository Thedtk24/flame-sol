# Profiling - Matrix System Resolution

Procedure for compilation, profiling, and performance analysis of the matrix system resolution program.

## 1. Compilation and Flamegraph Generation

### Prerequisites
Profiling was performed in a Linux environment (or Docker/VM) to leverage the `perf` tool and GLIBC symbols.
The FlameGraph generation scripts are required (available on [GitHub](https://github.com/brendangregg/FlameGraph)).
Assume the `FlameGraph` repository is cloned at the project root.

### Compilation
The project is compiled via the included `Makefile`, using `gcc` with debug flags (`-g`) and OpenMP optimization (`-fopenmp`).

```bash
make
```
This command generates the `solution` executable.

### Flamegraph Generation
The procedure relies on the `perf` tool (Linux) and the FlameGraph script suite.

1. **Performance Recording**:
   The executable is run under `perf` to capture stack traces.
   The `-g` option captures the call graph.

   ```bash
   sudo perf record -g ./solution
   ```
   *Note: This creates a `perf.data` file.*

2. **SVG File Creation**:
   The raw data is converted and processed by the `FlameGraph` perl scripts.

   ```bash
   perf script | ./Flamegraph/stackcollapse-perf.pl | ./Flamegraph/flamegraph.pl > sol-flamegraph.svg
   ```
