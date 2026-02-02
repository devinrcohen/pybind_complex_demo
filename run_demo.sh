#!/usr/bin/env bash
set -e
set -o pipefail

source "$(conda info --base)/etc/profile.d/conda.sh"
conda activate pybind-demo

# Build (optional; pip -e will build as needed, but explicit is fine)
BUILD_DIR="build-ninja"
cmake -S . -B "$BUILD_DIR" -G Ninja \
  -DPython3_EXECUTABLE="$CONDA_PREFIX/bin/python" \
  -DPython3_ROOT_DIR="$CONDA_PREFIX"
cmake --build "$BUILD_DIR" -v

# Install/update the editable wheel so the extension ends up in site-packages
pip install -e . --no-build-isolation -q

python -m pybind_complex_demo.demo
