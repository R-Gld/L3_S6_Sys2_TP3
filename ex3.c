#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct tfunc_param {
    int min;
    int max;
    int *arr;
    size_t arr_size;
};

void *tfunc(void *arg) {
    const struct tfunc_param param = *(struct tfunc_param *)arg;

    int *arr = param.arr;
    const size_t arr_size = param.arr_size;

    const int min = param.min;
    const int max = param.max;
    srand((unsigned int) time(NULL) ^ getpid());

    for (size_t i = 0; i < arr_size; ++i) {
        arr[i] = min + (int) (random() % (max - min +1));
    }
    return NULL;
}

int main(const int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <min> <max> <size>", argv[0]);
        return EXIT_FAILURE;
    }
    int min = atoi(argv[1]);
    int max = atoi(argv[2]);
    const int size = atoi(argv[3]);
    if (min == 0 || max == 0) {
        fprintf(stderr, "min/max must be an int greater than zero.");
        return EXIT_FAILURE;
    }
    if (max < min) {
        const int tmp = min;
        max = min;
        min = tmp;
    }

    int arr[size];

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
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    return EXIT_SUCCESS;
}