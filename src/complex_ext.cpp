#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <complex>
#include <utility> // pair, tuple
#include <stdexcept>

namespace py = pybind11;

// Explicit lane representation for NumPy complex64 (2 x float32).
// NumPy guarantees complex64 is stored as two float32 lanes: [real, imag].
struct c64_lane {
    float re;
    float im;
};

static inline std::complex<float> load_c64(const c64_lane& v) {
    return {v.re, v.im};
}

static inline c64_lane store_c64(const std::complex<float>& z) {
    return {z.real(), z.imag()};
}

// Require: 1D, C-contiguous, dtype=complex64, mutable.
static inline std::pair<c64_lane*, ssize_t> require_c_contig_complex64_mut(py::array x) {
    if (!py::dtype("complex64").is(x.dtype())) {
        throw std::invalid_argument("Expected dtype=complex64 (np.complex64).");
    }
    if (x.ndim() != 1) {
        throw std::invalid_argument("Expected a 1D array.");
    }
    if (!(x.flags() & py::array::c_style)) {
        throw std::invalid_argument("Expected a C-contiguous array.");
    }

    auto* ptr = static_cast<c64_lane*>(x.mutable_data());
    return {ptr, x.shape(0)};
}

// Require: 1D, C-contiguous, dtype=complex64, read-only ok.
static inline std::pair<const c64_lane*, ssize_t> require_c_contig_complex64_ro(py::array x) {
    if (!py::dtype("complex64").is(x.dtype())) {
        throw std::invalid_argument("Expected dtype=complex64 (np.complex64).");
    }
    if (x.ndim() != 1) {
        throw std::invalid_argument("Expected a 1D array.");
    }
    if (!(x.flags() & py::array::c_style)) {
        throw std::invalid_argument("Expected a C-contiguous array.");
    }

    auto* ptr = static_cast<const c64_lane*>(x.data());
    return {ptr, x.shape(0)};
}

// In-place: x *= alpha
void scale_inplace(py::array x, std::complex<float> alpha) {
    auto [ptr, n] = require_c_contig_complex64_mut(x);
    for (ssize_t i = 0; i < n; ++i) {
        auto z = load_c64(ptr[i]);
        z *= alpha;
        ptr[i] = store_c64(z);
    }
}

// Return: y = a*x + b (new array)
py::array axpb(py::array x, std::complex<float> a, std::complex<float> b) {
    auto [xptr, n] = require_c_contig_complex64_ro(x);

    // Allocate a 1D complex64 array; ShapeContainer avoids initializer-list ambiguity in pybind11 3.x
    py::array y(py::dtype("complex64"), py::array::ShapeContainer{n});
    auto* yptr = static_cast<c64_lane*>(y.mutable_data());

    for (ssize_t i = 0; i < n; ++i) {
        auto z = load_c64(xptr[i]);
        auto outv = a * z + b;
        yptr[i] = store_c64(outv);
    }
    return y;
}

// Return: elementwise multiply x*y (new array)
py::array mul(py::array x, py::array y) {
    auto [xptr, nx] = require_c_contig_complex64_ro(x);
    auto [yptr, ny] = require_c_contig_complex64_ro(y);
    if (nx != ny) {
        throw std::invalid_argument("Arrays must have the same length.");
    }

    py::array out(py::dtype("complex64"), py::array::ShapeContainer{nx});
    auto* optr = static_cast<c64_lane*>(out.mutable_data());

    for (ssize_t i = 0; i < nx; ++i) {
        auto zx = load_c64(xptr[i]);
        auto zy = load_c64(yptr[i]);
        optr[i] = store_c64(zx * zy);
    }
    return out;
}

PYBIND11_MODULE(complex_ext, m) {
    m.doc() = "pybind11 + numpy complex64 demo";

    m.def("scale_inplace", &scale_inplace,
          py::arg("x"), py::arg("alpha"),
          "In-place multiply: x *= alpha. Requires 1D C-contiguous complex64.");

    m.def("axpb", &axpb,
          py::arg("x"), py::arg("a"), py::arg("b"),
          "Return y = a*x + b. Requires 1D C-contiguous complex64.");

    m.def("mul", &mul,
          py::arg("x"), py::arg("y"),
          "Return elementwise multiply x*y. Requires 1D C-contiguous complex64.");
}
