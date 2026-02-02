import numpy as np
from pybind_complex_demo import scale_inplace, axpb, mul

def print_array(arr, delim="\n"):
    st= "["
    trim_len = len(st)
    for el in arr:
        st += str(el) + delim
    st = st[:-trim_len-1] + "]"
    print(st)

def main():
    x = (np.arange(8, dtype=np.float32) + 1j * np.arange(8, dtype=np.float32)).astype(np.complex64)
    print_array(x, ",\n")
    print(f"""
type: {x.dtype}
C_CONTIGUOUS: {x.flags["C_CONTIGUOUS"]}
""")


    scale_inplace(x, np.complex64(0.5 + 0.25j))
    print("scaled x:", x)

    y = axpb(x, np.complex64(2.0 + 0.0j), np.complex64(1.0 - 1.0j))
    print("y = 2x + (1-1j):", y)

    z = mul(x, y)
    print("z = x*y:", z)

if __name__ == "__main__":
    main()
