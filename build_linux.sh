#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"
g++ -std=c++17 -O2 -pthread -Wall -Wextra \
  src/backend/main.cpp src/backend/ThermalSolver.cpp \
  src/backend/Backpropagation.cpp src/backend/Parsers.cpp \
  src/backend/HardwareManager.cpp -o workbench
echo "Built ./workbench"
