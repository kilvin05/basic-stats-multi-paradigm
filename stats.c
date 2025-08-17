#include <stdio.h>
#include <stdlib.h>

/* Comparator for qsort (ascending) */
int cmp_int(const void *a, const void *b) {
    int ia = *(const int *)a, ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

double mean(const int *arr, size_t n) {
    long long sum = 0;
    for (size_t i = 0; i < n; i++) sum += arr[i];
    return (n == 0) ? 0.0 : (double)sum / (double)n;
}

double median(int *arr_copy, size_t n) {
    if (n == 0) return 0.0;
    qsort(arr_copy, n, sizeof(int), cmp_int);
    if (n % 2 == 1) return arr_copy[n/2];
    return (arr_copy[n/2 - 1] + arr_copy[n/2]) / 2.0;
}

/* Mode: prints all modes, returns how many modes; also returns the max frequency via *out_freq */
size_t mode(int *arr_copy, size_t n, int **out_modes, int *out_freq) {
    if (n == 0) { *out_modes = NULL; *out_freq = 0; return 0; }
    qsort(arr_copy, n, sizeof(int), cmp_int);

    int max_count = 1;
    int current = arr_copy[0], count = 1;

    /* First pass: find max frequency */
    for (size_t i = 1; i < n; i++) {
        if (arr_copy[i] == current) {
            count++;
        } else {
            if (count > max_count) max_count = count;
            current = arr_copy[i];
            count = 1;
        }
    }
    if (count > max_count) max_count = count;

    /* Second pass: collect all values with max_count */
    size_t modes_cap = 8, modes_len = 0;
    int *modes = (int *)malloc(modes_cap * sizeof(int));
    if (!modes) { perror("malloc"); exit(1); }

    current = arr_copy[0]; count = 1;
    for (size_t i = 1; i < n; i++) {
        if (arr_copy[i] == current) {
            count++;
        } else {
            if (count == max_count) {
                if (modes_len == modes_cap) {
                    modes_cap *= 2;
                    modes = (int *)realloc(modes, modes_cap * sizeof(int));
                    if (!modes) { perror("realloc"); exit(1); }
                }
                modes[modes_len++] = current;
            }
            current = arr_copy[i];
            count = 1;
        }
    }
    if (count == max_count) {
        if (modes_len == modes_cap) {
            modes_cap *= 2;
            modes = (int *)realloc(modes, modes_cap * sizeof(int));
            if (!modes) { perror("realloc"); exit(1); }
        }
        modes[modes_len++] = current;
    }

    *out_modes = modes;
    *out_freq = max_count;
    return modes_len;
}

void print_int_list(const int *arr, size_t n) {
    printf("[");
    for (size_t i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i + 1 < n) printf(", ");
    }
    printf("]");
}

int main(int argc, char **argv) {
    /* Example usage: ./stats 1 2 2 3 4 */
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <int1> <int2> ...\n", argv[0]);
        return 1;
    }

    size_t n = (size_t)(argc - 1);
    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) { perror("malloc"); return 1; }

    for (size_t i = 0; i < n; i++) arr[i] = atoi(argv[i + 1]);

    /* mean */
    double m = mean(arr, n);

    /* median (uses a copy since it sorts) */
    int *copy_for_median = (int *)malloc(n * sizeof(int));
    if (!copy_for_median) { perror("malloc"); free(arr); return 1; }
    for (size_t i = 0; i < n; i++) copy_for_median[i] = arr[i];
    double med = median(copy_for_median, n);

    /* mode (another copy) */
    int *copy_for_mode = (int *)malloc(n * sizeof(int));
    if (!copy_for_mode) { perror("malloc"); free(copy_for_median); free(arr); return 1; }
    for (size_t i = 0; i < n; i++) copy_for_mode[i] = arr[i];
    int *modes = NULL, freq = 0;
    size_t modes_len = mode(copy_for_mode, n, &modes, &freq);

    printf("Input: ");
    print_int_list(arr, n);
    printf("\nMean: %.4f\n", m);
    printf("Median: %.4f\n", med);
    printf("Mode(s) (freq=%d): ", freq);
    print_int_list(modes, modes_len);
    printf("\n");

    free(modes);
    free(copy_for_mode);
    free(copy_for_median);
    free(arr);
    return 0;
}
