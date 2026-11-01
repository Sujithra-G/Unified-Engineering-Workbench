# Unified Engineering Workbench — Technical Report
**Candidate:** [Your name / Candidate ID]  
**Platform target:** Windows 11 x64  
**Implementation language:** C++17 backend; HTML/CSS/JavaScript presentation prototype

## 1. Design and architecture
The prototype separates numerical and file-processing logic into C++ modules: thermal solver, parsers, parallel workload, hardware mocks and backpropagation. A static browser interface provides Geometry, Thermal Analysis, Performance and Hardware views. The current interface is not yet connected to the C++ process; backend commands are run separately. CMake provides a reproducible source build.

## 2. Thermal FEA
A uniform 1 m rod is divided into N two-node linear elements. Each element has stiffness matrix `(kA/h)[[1,-1],[-1,1]]` and consistent source load `qAh/2 [1,1]`. Global contributions are assembled and Dirichlet temperatures are imposed at both ends. The analytical solution is `T(x)=100-80x+[q/(2k)]x(1-x)` °C for the supplied L=1 m case. Lateral surfaces are insulated and material properties are constant. The output CSVs and plots are in `results/`.

For the constant source and constant conductivity case, the exact quadratic field is represented exactly at the nodes by linear finite elements; midpoint interpolation has a nonzero error because a straight line does not reproduce the quadratic curvature. The midpoint error should decrease as the mesh is refined. This is a useful distinction between nodal agreement and within-element interpolation error.

## 3. Performance
`workbench benchmark` creates 400 independent cases from fixed seed 20261031 with positive conductivity and heat generation. It runs 1–4 worker threads, capped by logical CPU count, and prints five-run medians. Timings are machine-dependent. Record CPU model, physical/logical cores, RAM, compiler/version, flags, warm-up policy and actual output before making performance claims. Parallel overhead can dominate small workloads due to thread creation, scheduling and memory/cache effects.

## 4. CAD / result visualization
The parser supports ASCII STL facet counting and CSV nodes with schema `node_id,x_m,y_m,z_m,temperature_C`. The supplied STL is a self-created one-triangle sample; the CSV is synthetic and is not an ANSYS export. The static UI is a presentation prototype; interactive 3D rendering, node inspection and STL rotation/pan/zoom are not yet integrated. Binary STL and native ANSYS formats are out of scope.

## 5. Embedded design
See `embedded_design.md`. MCU, FPGA and JTAG devices are represented by mock adapters. The timeout is recoverable and explicitly simulated. No real electrical connection or programming is performed.

## 6. Backpropagation
For `h=tanh(wx+b)`, `yhat=vh+c`, and `J=0.5(yhat-y)^2`, let `e=yhat-y`. Then `dJ/dv=e*h`, `dJ/dc=e`, `dJ/dw=e*v*(1-h²)*x`, and `dJ/db=e*v*(1-h²)`. All four parameters are updated simultaneously from gradients evaluated at the original parameter values. See `results/backprop_loss.csv` and `backprop_loss.png`. Verify all four gradients by central finite differences before submission.

## 7. Limitations and verification
This is a source prototype, not a verified final Windows package. The UI is not wired to the backend; no `Launch.exe` is supplied; clean-machine packaging and runtime bundling remain. STL visualization is not implemented in the UI. Benchmark hardware metadata must be filled from the actual test machine. Automated tests and the handwritten appendix must be completed and verified. Do not represent simulated hardware or synthetic CSV as physical/ANSYS evidence.
