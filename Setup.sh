#!/usr/bin/env bash
set -e
echo "Building kernel4d_sim..."
g++ -std=c++11 -O2 kernel4d_sim.cpp -o kernel4d_sim
echo "Build complete."
echo
echo "Running 4D Kernel simulation (local-only) ..."
echo "Note: this will create ./4d_kernel_env directory with fake artifacts."
echo
./kernel4d_sim
