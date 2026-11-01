# Unified Engineering Workbench
A C++17 engineering prototype for the PASS Science Works R&D internship assignment.

## Current status
Source prototype only. The C++ backend implements the 1D thermal solver, ASCII STL facet parser, temperature CSV parser, deterministic threaded batch benchmark, simulated MCU/FPGA/JTAG adapters and single-sample backpropagation. The browser UI is a static presentation prototype and is not yet connected to the backend.

## Prerequisites
- Windows 11 x64 (target), CMake 3.16+, and Visual Studio 2022 C++ workload; or
- Linux/macOS with a C++17 compiler and CMake for development testing.

## Build
From the project root:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
Run the generated `workbench` executable from the project root so relative `results/` paths resolve.

## Commands
```bash
workbench all
workbench thermal
workbench backprop
workbench benchmark
workbench hardware
workbench csv data/temperature_nodes.csv
workbench stl data/example_ascii.stl
```
On Windows, use `workbench.exe` in place of `workbench`.

## UI
Open `src/frontend/index.html` in a browser. It is a static UI prototype and does not launch or communicate with the C++ backend.

## Supported formats
- ASCII STL: facet counting only; binary STL is rejected.
- CSV: `node_id,x_m,y_m,z_m,temperature_C`.
- Synthetic sample CSV is not an actual ANSYS export.

## Tests and evidence
Run the smoke tests after configuring and building:
```bash
ctest --test-dir build --output-on-failure
```
Also run `workbench backprop` and `workbench benchmark` to verify all four finite-difference gradients and serial/parallel agreement. See `tests/test_plan.md`, `results/verification_evidence.md`, `results/`, and `docs/report.md`. Record actual machine/compiler metadata and benchmark output. Do not claim a clean-machine launch test until performed.

## Packaging limitations
No `Launch.exe` or self-contained Windows bundle is included. A Windows build, runtime dependency audit, one-click launcher, editor run configuration and clean-machine test remain necessary for full compliance. See the report for all known limitations.

