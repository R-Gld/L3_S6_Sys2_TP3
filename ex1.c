#include <stdio.h>
#include <stdlib.h>

int mult(const int fact, const int x) {
    return fact * x;
}

void map(int* arr, const int size, int (*func)(int, int), const int fact) {
    for (int i = 0; i < size; ++i) {
        const int vl = arr[i];
        arr[i] = func(fact, vl);
    }
}

int main(void) {
    int arr[] = { 1, 3, 5 };
    map(arr, 3, mult, 10);

    for (int i = 0; i < 3; ++i) {
        printf("%d\n", arr[i]);
    }

    return EXIT_SUCCESS;
}