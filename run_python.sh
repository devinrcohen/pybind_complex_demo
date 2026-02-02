#!/usr/bin/env bash
set -e
set -o pipefail

# Load conda into this shell (portable across machines)
source "$(conda info --base)/etc/profile.d/conda.sh"

# Activate env only if needed
if [ "${CONDA_DEFAULT_ENV:-}" != "pybind-demo" ]; then
  conda activate pybind-demo
fi

# Run using the installed/editable package (recommended)
python -m pybind_complex_demo.demo
