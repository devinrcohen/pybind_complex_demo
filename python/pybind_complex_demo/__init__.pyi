from __future__ import annotations
import numpy as np
from numpy.typing import NDArray

Complex64Vec = NDArray[np.complex64]

def scale_inplace(x: Complex64Vec, alpha: np.complex64) -> None: ...
def axpb(x: Complex64Vec, a: np.complex64, b: np.complex64) -> Complex64Vec: ...
def mul(x: Complex64Vec, y: Complex64Vec) -> Complex64Vec: ...

__all__: list[str]
