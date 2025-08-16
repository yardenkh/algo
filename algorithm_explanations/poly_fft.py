r"""\
Polynomial Multiplication with the Fast Fourier Transform (FFT)
===============================================================

This module shows how to multiply two polynomials efficiently using the
Cooley--Tukey Fast Fourier Transform. All explanations below are written
so you can copy them directly into a LaTeX document. Math expressions use
standard LaTeX syntax.

---------------------------------------------------------------
1. Polynomial Definitions
---------------------------------------------------------------
Let
\[
	f(x) = a_0 + a_1 x + a_2 x^2 + \dots + a_{n-1} x^{n-1}, \qquad
	g(x) = b_0 + b_1 x + b_2 x^2 + \dots + b_{m-1} x^{m-1}.
\]
Their product \(h(x) = f(x) g(x)\) has degree \((n-1)+(m-1) = n + m - 2\) and thus
\((n+m-1)\) coefficients:
\[
	h(x) = \sum_{k=0}^{n+m-2} c_k x^k.
\]
Each coefficient is the discrete convolution (Cauchy product):
\[
	c_k = \sum_{i+j = k} a_i b_j = \sum_{i=0}^{k} a_i b_{k-i}, \quad 0 \le k \le n+m-2,
\]
where we treat any \(a_i\) or \(b_j\) with out-of-range index as zero.

The naive algorithm computes every \(c_k\) with an inner sum: time complexity
\(\Theta(n m)\). For large inputs this is slow.

---------------------------------------------------------------
2. FFT Strategy (Evaluation / Pointwise Multiplication / Interpolation)
---------------------------------------------------------------
Key idea: pick an integer \(N\) (a power of two) with \(N \ge n + m - 1\). We will
work with *padded* coefficient arrays of length \(N\) by appending zeros.

Define the primitive \(N\)-th root of unity
\[
	\omega_N = e^{2\pi i / N}.
\]
We evaluate both polynomials at all powers \(\omega_N^k\), \(k=0,1,\dots,N-1\):
\[
	F_k = f(\omega_N^k), \qquad G_k = g(\omega_N^k).
\]
Then their product values are
\[
	H_k = F_k G_k = f(\omega_N^k) g(\omega_N^k) = h(\omega_N^k).
\]
Having all \(H_k\), we recover the coefficients of \(h\) by the *inverse* discrete Fourier transform.

The FFT and inverse FFT compute these transforms in \(O(N \log N)\) instead of \(O(N^2)\).

---------------------------------------------------------------
3. Even / Odd Decomposition (Why Recursion Works)
---------------------------------------------------------------
Any polynomial \(P(x)\) of degree < \(N\) can be split:
\[
	P(x) = P_{\text{even}}(x^2) + x\, P_{\text{odd}}(x^2),
\]
where
\[
	P_{\text{even}}(t) = a_0 + a_2 t + a_4 t^2 + \dots, \qquad
	P_{\text{odd}}(t)  = a_1 + a_3 t + a_5 t^2 + \dots
\]
Evaluating at \(x = \omega_N^k\):
\[
	P(\omega_N^k) = P_{\text{even}}(\omega_N^{2k}) + \omega_N^k P_{\text{odd}}(\omega_N^{2k}).
\]
Because \(\omega_N^{2} = \omega_{N/2}\), each size-\(N\) transform reduces to two size-\(N/2\) transforms.

---------------------------------------------------------------
4. Forward vs Inverse Transform
---------------------------------------------------------------
The (forward) DFT of a length-\(N\) sequence \(a_0,\dots,a_{N-1}\) is
\[
	A_k = \sum_{j=0}^{N-1} a_j \omega_N^{jk}, \qquad 0 \le k < N.
\]
The inverse DFT is
\[
	a_j = \frac{1}{N} \sum_{k=0}^{N-1} A_k \omega_N^{-jk}.
\]
Difference: complex exponent sign and final division by \(N\). We re-use the same recursive code,
switching the sign to compute inverse; then divide by \(N\) once at the end.

---------------------------------------------------------------
5. Algorithm Steps Summary
---------------------------------------------------------------
1. Read coefficient lists A, B.
2. Let need = n + m - 1.
3. Choose N = smallest power of two with N >= need.
4. Zero-pad A, B to length N.
5. Compute FFT(A) -> F, FFT(B) -> G.
6. Pointwise multiply: H_k = F_k * G_k.
7. Inverse FFT(H) -> h (length N complex values; should be nearly real).
8. Round small floating errors; keep first 'need' coefficients.

---------------------------------------------------------------
6. Complexity
---------------------------------------------------------------
Time: \(O(N \log N)\) with N the chosen power of two (at least n+m-1).  Space: \(O(N)\).

---------------------------------------------------------------
7. Practical Notes
---------------------------------------------------------------
- Floating rounding introduces tiny imaginary parts (~1e-12). We discard them.
- For exact huge integer multiplication use multiple mod primes (NTT) or arbitrary precision.
- This code favors clarity over micro-optimizations.

---------------------------------------------------------------
8. Minimal Example (Same as C example)
---------------------------------------------------------------
Multiply:
\[
	A(x) = 3 + 2x + 5x^2, \qquad B(x) = 5 + 1x + 2x^2 + 3x^3.
\]
Then need = 3 + 4 - 1 = 6. Next power of two: 8.
Result:
\[
	A(x) B(x) = 15 + 13x + 33x^2 + 18x^3 + 16x^4 + 15x^5.
\]

You can verify the first few coefficients manually:
\(c_0 = 3*5 = 15\),
\(c_1 = 3*1 + 2*5 = 13\),
\(c_2 = 3*2 + 2*1 + 5*5 = 6 + 2 + 25 = 33\), etc.

---------------------------------------------------------------
END OF EXPLANATION
"""

from __future__ import annotations

from typing import List, Sequence
import cmath
import math


def _fft(a: List[complex], invert: bool = False) -> None:
    """In-place Cooley--Tukey FFT.

    Parameters
    ----------
    a : list[complex]
                    Data (length must be a power of two).
    invert : bool
                    False => forward FFT, True => inverse FFT.

    Notes
    -----
    Uses recursion (clearer, not the fastest). When invert is True we use the
    negative angle and divide by 2 after each combine so that at the top level
    the result is already scaled by 1/N.
    """
    n = len(a)
    if n == 1:
        return
    even = a[0::2]
    odd = a[1::2]
    _fft(even, invert)
    _fft(odd, invert)
    ang = (2 * math.pi / n) * (-1 if invert else 1)
    w = 1 + 0j
    wn = cmath.exp(1j * ang)
    for k in range(n // 2):
        u = even[k]
        v = odd[k] * w
        a[k] = u + v
        a[k + n // 2] = u - v
        if invert:
            # When inverting, scale each level by 1/2 so that total is 1/n.
            a[k] /= 2
            a[k + n // 2] /= 2
        w *= wn


def multiply_polynomials(A: Sequence[float], B: Sequence[float]) -> List[float]:
    """Return the coefficient list of the product polynomial A(x)*B(x).

    Parameters
    ----------
    A, B : sequences of numbers (real) representing coefficients
                    A[i] is coefficient of x^i.
    Returns
    -------
    list[float]
                    Coefficients of the product (length len(A)+len(B)-1).
    """
    n = len(A)
    m = len(B)
    need = n + m - 1
    N = 1
    while N < need:
        N <<= 1
    fa = list(map(complex, A)) + [0j] * (N - n)
    fb = list(map(complex, B)) + [0j] * (N - m)
    _fft(fa, invert=False)
    _fft(fb, invert=False)
    for i in range(N):
        fa[i] *= fb[i]
    _fft(fa, invert=True)
    # Extract real parts and round small noise
    result = [
        float(round(fa[i].real if abs(fa[i].imag) < 1e-9 else fa[i].real, 12))
        for i in range(need)
    ]
    return result


def format_polynomial(coeffs: Sequence[float]) -> str:
    """Format coefficients as a human-readable polynomial string."""
    terms: List[str] = []
    for i, c in enumerate(coeffs):
        if abs(c) < 1e-12:
            continue
        if i == 0:
            terms.append(f"{c:g}")
        elif i == 1:
            terms.append(f"{c:g}x")
        else:
            terms.append(f"{c:g}x^{i}")
    return " + ".join(terms) if terms else "0"


if __name__ == "__main__":
    A = [3, 2, 5]
    B = [5, 1, 2, 3]
    C = multiply_polynomials(A, B)
    print("A:", A)
    print("B:", B)
    print("Product coefficients:", C)
    print("Polynomial:", format_polynomial(C))
