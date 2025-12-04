# 4D-Kernel-Sim

**WARNING — HARmless Simulation Only.**  
This repository contains a theatrical, local-only simulation of a fictional "4D Kernel" setup and boot process. It does **not** touch your system kernel, does **not** load any kernel modules, and requires **no root** access. Everything runs inside a working directory that the program creates.

The intent is purely educational/entertainment — to produce convincing-looking console output for demos or pranks without performing any real low-level or network operations.

---

## Files

- `README.md` — this file.
- `kernel4d_sim.cpp` — the main C++ simulator (compile with g++).
- `setup.sh` — build & run helper script.
- `4d-kernel.service.sample` — sample systemd unit (commented; do not enable unless you understand it).

---

## Requirements

- A modern C++ compiler (g++/clang++) supporting C++11 or later.
- POSIX-compatible shell for `setup.sh`.

Tested with:
- Ubuntu 20.04+, g++ 9+
- macOS/Windows (msys) terminal — colors may vary.

---

## Build & Run

Make executable and run the helper script:

```bash
chmod +x setup.sh
./setup.sh
