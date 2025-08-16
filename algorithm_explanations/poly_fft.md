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

def _fft(a: List[complex], invert: bool = False) -> None:
    n = len(a)
    if n == 1:
        return
    even = a[0::2]
    odd = a[1::2]
    _fft(even, invert)
    _fft(odd, invert)
    ang = (2 * math.pi / n) * (-1 if invert else 1)
    w = 1+0j
    wn = cmath.exp(1j * ang)
    for k in range(n // 2):
        u = even[k]
        v = odd[k] * w
        a[k] = u + v
        a[k + n//2] = u - v
        if invert:
            a[k] /= 2
            a[k + n//2] /= 2
        w *= wn

def multiply_polynomials(A: Sequence[float], B: Sequence[float]) -> List[float]:
    n, m = len(A), len(B)
    need = n + m - 1
    N = 1
    while N < need:
        N <<= 1
    fa = list(map(complex, A)) + [0j]*(N - n)
    fb = list(map(complex, B)) + [0j]*(N - m)
    _fft(fa, False)
    _fft(fb, False)
    for i in range(N):
        fa[i] *= fb[i]
    _fft(fa, True)
    return [float(round(fa[i].real, 12)) for i in range(need)]

if __name__ == "__main__":
    A = [3, 2, 5]
    B = [5, 1, 2, 3]
    print(multiply_polynomials(A, B))  # [15.0, 13.0, 33.0, 18.0, 16.0, 15.0]
```

---

## 9. Notes & Extensions

- Use an iterative in-place FFT for performance-critical production code.
- For large exact integer multiplication, consider the Number Theoretic Transform.
- Bluestein's algorithm handles lengths not convenient for power-of-two padding.

---

**Summary:** FFT converts convolution into pointwise multiplication in the frequency domain, reducing polynomial multiplication from quadratic to quasi-linear time.
