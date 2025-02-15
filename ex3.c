#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/**
 * Convert a string to a long but with the securities of strtol(3) instead of atoi(3).
 *
 * @param arg a string.
 * @return the conversion of this string in long.
 */
long convertWithCheck(const char *arg);

/**
 *
 * @param arg Should be a pointer to a struct tfunc_param
 * @return NULL
 */
void *tfunc(void *arg);

struct tfunc_param {
    long min;
    long max;
    long *arr;
    size_t arr_size;
};


int main(const int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <min> <max> <size>", argv[0]);
        return EXIT_FAILURE;
    }
    long min = convertWithCheck(argv[1]);
    long max = convertWithCheck(argv[2]);
    const long size = convertWithCheck(argv[3]);
    if (min == 0 || max == 0) {
        fprintf(stderr, "min/max must be an int greater than zero.");
        return EXIT_FAILURE;
    }
    if (max < min) {
        const long tmp = min;
        max = min;
        min = tmp;
    }

    long arr[size];

    struct tfunc_param param;
    param.min = min;
    param.max = max;
    param.arr = arr;
    param.arr_size = size;

    pthread_t tid;
    int err = pthread_create(&tid, NULL, tfunc, &param);
    if (err != 0) {
        errno = err;
        perror("pthread_create");
        return EXIT_FAILURE;
    }
    err = pthread_join(tid, NULL);
    if (err != 0) {
        errno = err;
        perror("pthread_join");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < size; ++i) {
        printf("arr[%d] = %lu\n", i, arr[i]);
    }

    return EXIT_SUCCESS;
}

long convertWithCheck(const char *arg) {
    char *endPointer;
    const long result = strtol(arg, &endPointer, 10);
    if ((errno == ERANGE && (result == LONG_MAX || result == LONG_MIN)) || (errno != 0 && result == 0)) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

    if (endPointer == arg) {
        fprintf(stderr, "No Digits were found\n");
        exit(EXIT_FAILURE);
    }

    if (*endPointer != '\0') {
        fprintf(stderr, "Further characters after number: %s\n", endPointer);
        exit(EXIT_FAILURE);
    }
    return result;
}

void *tfunc(void *arg) {
    const struct tfunc_param param = *(struct tfunc_param *)arg;

    long *arr = param.arr;
    const size_t arr_size = param.arr_size;

    const long min = param.min;
    const long max = param.max;
    srand((unsigned int) time(NULL) ^ getpid());

    for (size_t i = 0; i < arr_size; ++i) {
        arr[i] = min + random() % (max - min +1);
    }
    return NULL;
}