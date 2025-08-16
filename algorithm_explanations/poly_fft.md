# Fast Polynomial Multiplication with FFT

This document explains (with LaTeX-ready math) how the Cooley–Tukey Fast Fourier Transform (FFT) lets us multiply polynomials faster than the naive convolution. Copy/paste directly into any Markdown renderer that supports MathJax / KaTeX (GitHub works with `$...$` and `$$...$$`).

---

## 1. Problem Statement

Given two polynomials

$$
f(x) = a_0 + a_1 x + a_2 x^2 + \dots + a_{n-1} x^{n-1}, \qquad
g(x) = b_0 + b_1 x + b_2 x^2 + \dots + b_{m-1} x^{m-1}
$$

Their product has degree $(n-1)+(m-1)=n+m-2$ and thus $(n+m-1)$ coefficients:

$$
 h(x) = f(x)g(x) = \sum_{k=0}^{n+m-2} c_k x^k.
$$

Each coefficient is the Cauchy (discrete) convolution:

$$
 c_k = \sum_{i+j=k} a_i b_j = \sum_{i=0}^{k} a_i b_{k-i}, \qquad 0 \le k \le n+m-2.
$$

Naive time complexity: $\Theta(n m)$.

---

## 2. FFT High-Level Strategy

We choose a power-of-two length $N \ge n+m-1$ and zero‑pad both coefficient arrays to length $N$.

1. Evaluate $f$ and $g$ at the $N$-th roots of unity: $\omega_N^k = e^{2\pi i k / N}$.
2. Multiply those values pointwise: $H_k = f(\omega_N^k) g(\omega_N^k)$.
3. Interpolate (inverse FFT) to get the coefficients of $h$.

Because the FFT and its inverse compute these transforms in $O(N\log N)$, for large inputs this beats $O(nm)$.

---

## 3. Roots of Unity

Let the primitive $N$-th root of unity be

$$
\omega_N = e^{2\pi i / N}.
$$

The set $\{1, \omega_N, \omega_N^2,\dots, \omega_N^{N-1}\}$ are the distinct roots of $z^N - 1 = 0$, and they satisfy symmetry relations (e.g. $\omega_N^{k+N/2} = -\omega_N^k$ when $N$ is even) that the FFT exploits.

---

## 4. Even–Odd Decomposition (Key Recurrence)

Any polynomial $P(x)$ (degree < $N$) can be written

$$
P(x) = P_{\text{even}}(x^2) + x \cdot P_{\text{odd}}(x^2)
$$

where

$$
P_{\text{even}}(t)= a_0 + a_2 t + a_4 t^2 + \dots, \qquad P_{\text{odd}}(t)= a_1 + a_3 t + a_5 t^2 + \dots
$$

Evaluating at $x = \omega_N^k$ gives

$$
P(\omega_N^k) = P_{\text{even}}(\omega_N^{2k}) + \omega_N^k P_{\text{odd}}(\omega_N^{2k}).
$$

But $\omega_N^{2} = \omega_{N/2}$, so we reduce one size-$N$ problem to two size-$N/2$ problems (divide & conquer). This is the essence of Cooley–Tukey.

---

## 5. Forward and Inverse DFT

Forward DFT:

$$
A_k = \sum_{j=0}^{N-1} a_j \omega_N^{jk}, \qquad 0 \le k < N.
$$

Inverse DFT:

$$
a_j = \frac{1}{N} \sum_{k=0}^{N-1} A_k \omega_N^{-jk}.
$$

Difference: sign of the angle and the $1/N$ factor.

---

## 6. Full FFT Multiplication Procedure

Given coefficient lists A, B:

1. Let $\text{need} = n + m - 1$.
2. Pick smallest $N=2^p \ge \text{need}$.
3. Zero‑pad A, B to length $N$.
4. Compute $\text{FFT}(A) = F$, $\text{FFT}(B) = G$.
5. For each $k$: $H_k = F_k G_k$.
6. Compute inverse FFT of $H$ to get $h$.
7. Divide each entry by $N$ (if not already scaled) and round tiny imaginary / floating noise.
8. Keep first $\text{need}$ real parts as the product coefficients.

Complexity: $O(N \log N)$ time, $O(N)$ space.

---

## 7. Worked Example

Multiply

$$
A(x) = 3 + 2x + 5x^2, \qquad B(x) = 5 + x + 2x^2 + 3x^3.
$$

Lengths: $n=3$, $m=4$, so $\text{need}=3+4-1=6$. Next power of two: $N=8$. After executing the FFT algorithm we obtain:

$$
A(x)B(x) = 15 + 13x + 33x^2 + 18x^3 + 16x^4 + 15x^5.
$$

Manual check of first coefficients:

$$
\begin{aligned}
 c_0 &= 3\cdot 5 = 15,\\
 c_1 &= 3\cdot 1 + 2\cdot 5 = 13,\\
 c_2 &= 3\cdot 2 + 2\cdot 1 + 5\cdot 5 = 6 + 2 + 25 = 33.
\end{aligned}
$$

---

## 8. Python Reference Implementation

Below is a clear recursive FFT implementation (educational, not micro‑optimized):

```python
from __future__ import annotations
import cmath, math
from typing import List, Sequence

############################################################
# RECURSIVE FFT (COOLEY–TUKEY) – FULLY ANNOTATED VERSION   #
############################################################
# Representation:
#   A polynomial a(x) = a0 + a1 x + ... + a_{n-1} x^{n-1}
#   is represented by a Python list a = [a0, a1, ..., a_{n-1}].
#
# Goal:
#   Efficiently compute the Discrete Fourier Transform (DFT)
#   values A_k = sum_{j=0}^{n-1} a_j * omega^{j k}, where omega = e^{2πi/n}.
#   The recursive FFT reduces O(n^2) to O(n log n) by splitting into
#   even / odd indices and reusing the (n/2)-size transforms.
#
# invert=False  -> forward FFT
# invert=True   -> inverse FFT (complex conjugate / divide by 2 at each merge)
#
# In-place strategy:
#   The list 'a' is *overwritten* with its DFT values (or inverse) to avoid
#   allocating large temporary arrays at every recursion layer.

def _fft(a: List[complex], invert: bool = False) -> None:
    """In-place recursive FFT.

    Args:
        a: list of complex numbers (length must be a power of two)
        invert: if True, performs inverse FFT.

    Side-effects:
        Mutates 'a' to contain its forward (or inverse) DFT.
    """
    n = len(a)
    # Base case: length-1 list is already its own DFT.
    if n == 1:
        return

    # Split into even-indexed and odd-indexed parts (stride slicing).
    even = a[0::2]          # elements a[0], a[2], a[4], ...
    odd = a[1::2]           # elements a[1], a[3], a[5], ...

    # Recurse on halves (each of size n/2).
    _fft(even, invert)
    _fft(odd, invert)

    # Fundamental n-th root of unity (direction depends on forward/inverse).
    # For inverse we use exp(-2πi / n), for forward exp(+2πi / n) *BUT*
    # we incorporate the sign by flipping with ( -1 if invert else 1 ).
    ang = (2 * math.pi / n) * (-1 if invert else 1)
    wn = cmath.exp(1j * ang)  # principal root
    w = 1 + 0j                # current power of wn

    # Merge step: combine results from even & odd parts.
    half = n // 2
    for k in range(half):
        # u = contribution from even part
        u = even[k]
        # v = contribution from odd part multiplied by current twiddle factor
        v = odd[k] * w
        # a[k]      = E_k + w^k * O_k
        a[k] = u + v
        # a[k+half] = E_k - w^k * O_k   (symmetry exploiting ω^{k+ n/2} = -ω^k)
        a[k + half] = u - v
        # For inverse FFT we normalize *gradually* to avoid a later full pass.
        if invert:
            a[k] /= 2
            a[k + half] /= 2
        # Advance twiddle factor: w := w * wn
        w *= wn

############################################################
# POLYNOMIAL MULTIPLICATION USING FFT                       #
############################################################
def multiply_polynomials(A: Sequence[float], B: Sequence[float]) -> List[float]:
    """Multiply two real-coefficient polynomials using FFT.

    Given A (length n) and B (length m):
        Result length = n + m - 1.
    Steps:
        1. Choose power-of-two N >= n + m - 1.
        2. Zero-pad A and B to length N.
        3. Forward FFT both arrays.
        4. Pointwise multiply spectra.
        5. Inverse FFT the product spectrum.
        6. Extract real parts (round tiny floating noise) of first (n+m-1) terms.
    """
    n, m = len(A), len(B)
    need = n + m - 1              # number of coefficients in product

    # Small optimization: handle trivial cases early.
    if n == 0 or m == 0:
        return []
    if n == 1:
        return [A[0] * b for b in B]
    if m == 1:
        return [B[0] * a for a in A]

    # Find smallest power of two >= need.
    N = 1
    while N < need:
        N <<= 1

    # Prepare complex copies padded with zeros.
    fa = list(map(complex, A)) + [0j] * (N - n)
    fb = list(map(complex, B)) + [0j] * (N - m)

    # Forward FFT both lists.
    _fft(fa, invert=False)
    _fft(fb, invert=False)

    # Pointwise multiply: corresponds to convolution in coefficient space.
    for i in range(N):
        fa[i] *= fb[i]

    # Inverse FFT to interpolate coefficients back.
    _fft(fa, invert=True)

    # Extract real parts (imag part should be ~0; rounding removes noise).
    result = [float(round(fa[i].real, 12)) for i in range(need)]
    return result

############################################################
# NAIVE REFERENCE (O(n*m)) FOR VALIDATION                  #
############################################################
def multiply_polynomials_naive(A: Sequence[float], B: Sequence[float]) -> List[float]:
    """Quadratic-time convolution for cross-checking correctness."""
    if not A or not B:
        return []
    res = [0.0] * (len(A) + len(B) - 1)
    for i, a in enumerate(A):
        for j, b in enumerate(B):
            res[i + j] += a * b
    return res

############################################################
# DEMONSTRATION / SELF-TEST                                #
############################################################
if __name__ == "__main__":
    # Example from the worked section:
    A = [3, 2, 5]                # 3 + 2x + 5x^2
    B = [5, 1, 2, 3]             # 5 + 1x + 2x^2 + 3x^3

    fft_product = multiply_polynomials(A, B)
    naive_product = multiply_polynomials_naive(A, B)

    print("FFT product   :", fft_product)
    print("Naive product :", naive_product)
    print("Match?", fft_product == naive_product)

    # Additional edge cases:
    print("Single-term * poly:", multiply_polynomials([7], [1, -4, 3]))
    print("Zero poly:", multiply_polynomials([], []))
    print("Negative coefficients:", multiply_polynomials([1, -2, 3], [-1, 4]))

    # Large random test (optional quick sanity) – commented out to keep deterministic output.
    # import random
    # X = [random.randint(-5,5) for _ in range(16)]
    # Y = [random.randint(-5,5) for _ in range(16)]
    # assert multiply_polynomials(X, Y) == multiply_polynomials_naive(X, Y)
    # print("Random test passed!")
```

### Line-by-Line Highlights

| Code Fragment                   | Explanation                                                         |
| ------------------------------- | ------------------------------------------------------------------- |
| `even = a[0::2]`                | Takes all even-indexed coefficients (recursively represent P_even). |
| `odd = a[1::2]`                 | Takes all odd-indexed coefficients (recursively represent P_odd).   |
| `ang = (2*pi/n)*(...)`          | Angle for primitive n-th root. Sign flips for inverse.              |
| `wn = exp(i*ang)`               | Principal root; successive powers multiply by `wn`.                 |
| Loop `for k in range(n//2)`     | Combine size-n/2 FFTs for even/odd to size-n FFT.                   |
| `a[k] = u+v`, `a[k+half] = u-v` | Butterfly operation exploiting symmetry.                            |
| Inverse branch divides by 2     | Deferred normalization so total factor becomes 1/n.                 |
| Pointwise multiply spectra      | Implements convolution theorem.                                     |
| Round to 12 decimals            | Removes small floating noise (e.g. -1e-13).                         |

### Correctness Sanity Checks

1. Length of result = `len(A)+len(B)-1`.
2. Leading zeros naturally appear if highest-degree terms cancel; we keep them unless trimming is desired.
3. Naive vs FFT method comparison ensures implementation integrity.

### Edge Cases Discussed

| Scenario                   | Behavior                                            |
| -------------------------- | --------------------------------------------------- |
| One operand empty          | Returns empty list.                                 |
| Single coefficient \* poly | Scales the polynomial.                              |
| Negative coefficients      | Handled seamlessly (complex arithmetic unaffected). |
| Floating inputs            | Works; output rounding mitigates precision noise.   |

### Complexity Recap

Let `N` be the power-of-two padding length. The recursion satisfies:
`T(N) = 2 T(N/2) + O(N)` → `T(N) = O(N log N)`.
Pointwise multiply adds O(N). Overall O(N log N). Space O(N) excluding recursion stack.

### When to Prefer Other Methods

| Input Size                        | Recommended Method                 |
| --------------------------------- | ---------------------------------- |
| Very small (N < ~32)              | Naive convolution (lower constant) |
| Medium floating point             | FFT as above                       |
| Large integer (exact)             | Number Theoretic Transform (NTT)   |
| Arbitrary length non-power-of-two | Bluestein or mixed-radix FFT       |

### Practical Tips

- For production, use an iterative bit-reversal FFT to reduce allocations.
- For integer multiplication, choose modulus primes supporting required roots (NTT).
- To reduce floating error, optionally use double padding and Kahan summation (rarely needed here).
- Post-processing: optionally strip trailing ~0 coefficients: `while res and abs(res[-1])<1e-12: res.pop()`.

### Mini Verification Example (Manual)

Take A(x)=1+2x, B(x)=1+3x+4x^2. Product should be:
`(1)(1) + (1)(3)+(2)(1) x + (1)(4)+(2)(3) x^2 + (2)(4) x^3 = 1 + 5x + 10x^2 + 8x^3`.
`multiply_polynomials([1,2],[1,3,4]) -> [1.0, 5.0, 10.0, 8.0]`.

---

## 9. Notes & Extensions

- Use an iterative in-place FFT for performance-critical production code.
- For large exact integer multiplication, consider the Number Theoretic Transform.
- Bluestein's algorithm handles lengths not convenient for power-of-two padding.

---

**Summary:** FFT converts convolution into pointwise multiplication in the frequency domain, reducing polynomial multiplication from quadratic to quasi-linear time.
