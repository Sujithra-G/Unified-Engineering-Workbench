# Verification evidence — actual development run

Environment: Linux C++ development container; GNU g++ invocation below. Exact host CPU/RAM inventory was not captured, so no hardware identity is claimed.

Build command:
```bash
g++ -std=c++17 -O2 -pthread -Wall -Wextra \
  src/backend/main.cpp src/backend/ThermalSolver.cpp \
  src/backend/Backpropagation.cpp src/backend/Parsers.cpp \
  src/backend/HardwareManager.cpp -o workbench
```
Build succeeded with exit code 0 and no compiler diagnostics.

Executed successfully with exit code 0:
- `workbench thermal`: midpoint errors N=10 1.25 C; N=20 0.3125 C; N=40 0.078125 C; N=80 0.01953125 C.
- `workbench backprop`: all four central finite-difference gradient absolute errors were below 1.2e-10, using epsilon=1e-6.
- `workbench benchmark`: serial and parallel results agreed exactly in this run (maximum absolute difference 0 C). Five measured repetitions per mode; warm-up excluded. Example medians: serial 0.743800 ms; 1 worker 0.793715 ms; 2 workers 0.668091 ms; 3 workers 0.754507 ms; 4 workers 0.781928 ms. Small workload overhead dominates; no claim of guaranteed speedup.
- `workbench hardware`: all three simulated mock adapters reported telemetry; recoverable FPGA timeout path displayed.
- `workbench csv data/temperature_nodes.csv`: validated 5 nodes.
- `workbench stl data/example_ascii.stl`: counted 1 ASCII facet.

These are development-container results, not Windows target-machine results. The application still has no Windows Launch.exe or bundled standalone package; the web UI is static and disconnected from the backend; STL and CSV 3D visualization is not implemented; no automated GUI test, actual screenshot walkthrough/video, or handwritten appendix is included. Candidate must add genuine local Git development evidence and actual Windows hardware/compiler details rather than invent them.
