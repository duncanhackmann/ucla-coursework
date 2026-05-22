# Parallel Lab — OpenMP Image Processing

Parallelized a sequential image-processing pipeline using OpenMP.

## Phases

### Phase 1 — Mean Pixel (bug fix + parallelization)
Computes the mean value for each color channel across all pixels. The sequential version contained a **data race**: multiple threads wrote to shared accumulators without synchronization. Fixed using `#pragma omp parallel for reduction(+:channel_sums[:NUM_CHANNELS])`.

### Phase 2 — Grayscale Max (parallelization)
Converts the image to grayscale, then finds the maximum grayscale value and counts how many times it appears. Parallelized with a custom reduction over two variables (`max_val` and `count`).

### Phase 3 — Gaussian Blur (parallelization)
Applies a convolution kernel (3×3 or 5×5 Gaussian) to the image. Parallelized the outer loop with `#pragma omp parallel for`, which is safe because each output pixel depends only on input pixels (no write-after-read hazards between iterations).

## Files

| File | Contents |
|------|----------|
| `parallel.c` | OpenMP implementations (the primary deliverable) |
| `sequential.c` | Original sequential reference code |
| `main.c` | Test harness with timing |
| `utils.h` | Image data types and constants |

## Building

```bash
make
./parallel_lab -p 1    # run phase 1 only
./parallel_lab          # run all phases
```
