#!/usr/bin/env bash
set -e
set -o pipefail

# Load conda function into this shell
if [ -f /usr/etc/profile.d/conda.sh ]; then
  source /usr/etc/profile.d/conda.sh
elif [ -f /home/devinrcohen/.conda/etc/profile.d/conda.sh ]; then
  source /home/devinrcohen/.conda/etc/profile.d/conda.sh
else
  echo "ERROR: Could not find conda.sh"
  exit 1
fi

# Activate env only if needed
if [ "${CONDA_DEFAULT_ENV:-}" != "pybind-demo" ]; then
  conda activate pybind-demo
fi

# Always (re)configure the build dir with Ninja so the build system is consistent
cmake -S . -B build -G Ninja \
  -DPython3_EXECUTABLE="$CONDA_PREFIX/bin/python" \
  -DPython3_ROOT_DIR="$CONDA_PREFIX"

cmake --build build -v

PYTHONPATH="$PWD/python" python -m pybind_complex_demo.demo
