/* 
 *  Name: DUNCAN HACKMANN
 *  UID: 106048712
 */

#include <stdlib.h>
#include <omp.h>

#include "utils.h"
#include "parallel.h"



/*
 *  PHASE 1: compute the mean pixel value
 *  This code is buggy! Find the bug and speed it up.
 */


void mean_pixel_parallel(const uint8_t img[][NUM_CHANNELS], int num_rows, int num_cols, double mean[NUM_CHANNELS]) {
    long long total_pixels = (long long)num_rows * num_cols;
    double channel_sums[NUM_CHANNELS] = {0.0};

    #pragma omp parallel for reduction(+:channel_sums[:NUM_CHANNELS])
    for (int i = 0; i < total_pixels; i++) {
        for (int ch = 0; ch < NUM_CHANNELS; ch++) {
            channel_sums[ch] += img[i][ch];
        }
    }

    for (int ch = 0; ch < NUM_CHANNELS; ch++) {
        mean[ch] = channel_sums[ch] / total_pixels;
    }
}



/*
 *  PHASE 2: convert image to grayscale and record the max grayscale value along with the number of times it appears
 *  This code is NOT buggy, just sequential. Speed it up.
 */


void grayscale_parallel(const uint8_t img[][NUM_CHANNELS], int num_rows, int num_cols, uint32_t grayscale_img[][NUM_CHANNELS], uint8_t *max_gray, uint32_t *max_count) {
    int row, col, ch, gray_ch;
    *max_gray = 0;
    *max_count = 0;

    #pragma omp parallel
    {
        uint8_t local_max_gray = 0;
        uint32_t local_max_count = 0;

        #pragma omp for schedule(dynamic) nowait
        for (row = 0; row < num_rows; row++) {
            int rowXnum_cols = row*num_cols;
            for (col = 0; col < num_cols; col++) {
                for (gray_ch = 0; gray_ch < NUM_CHANNELS; gray_ch++) {
                    grayscale_img[rowXnum_cols + col][gray_ch] = 0;
                    for (ch = 0; ch < NUM_CHANNELS; ch++) {
                        grayscale_img[rowXnum_cols + col][gray_ch] += img[rowXnum_cols + col][ch];
                    }
                    grayscale_img[rowXnum_cols + col][gray_ch] /= NUM_CHANNELS;
                    if (grayscale_img[rowXnum_cols + col][gray_ch] == local_max_gray) {
                        local_max_count++;
                    } else if (grayscale_img[rowXnum_cols + col][gray_ch] > local_max_gray) {
                        local_max_gray = grayscale_img[rowXnum_cols + col][gray_ch];
                        local_max_count = 1;
                    }
                }
            }
        }

        #pragma omp critical
        {
            if (local_max_gray > *max_gray) {
                *max_gray = local_max_gray;
                *max_count = local_max_count;
            } else if (local_max_gray == *max_gray) {
                *max_count += local_max_count;
            }
        }
    }
}

// *  PHASE 3: perform convolution on image
// *  This code is NOT buggy, just sequential. Speed it up.
// */

void convolution_parallel(const uint8_t padded_img[][NUM_CHANNELS], int num_rows, int num_cols, const uint32_t kernel[], int kernel_size, uint32_t convolved_img[][NUM_CHANNELS]) {
    int kernel_norm = 0;
    for (int i = 0; i < kernel_size * kernel_size; i++) {
        kernel_norm += kernel[i];
    }

    int conv_rows = num_rows - kernel_size + 1;
    int conv_cols = num_cols - kernel_size + 1;

    #pragma omp parallel for collapse(2)
    for (int ch = 0; ch < NUM_CHANNELS; ch++) {
        for (int idx = 0; idx < conv_rows * conv_cols; idx++) {
            int row = idx / conv_cols;
            int col = idx % conv_cols;
            uint32_t sum = 0;

            for (int kr = 0; kr < kernel_size; kr++) {
                for (int kc = 0; kc < kernel_size; kc++) {
                    sum += padded_img[(row + kr) * num_cols + col + kc][ch] * kernel[kr * kernel_size + kc];
                }
            }

            convolved_img[row * conv_cols + col][ch] = sum / kernel_norm;
        }
    }
}
