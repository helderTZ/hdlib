#include "timeit.h"

#include <stdio.h>
#include <unistd.h>

int main() {
    auto ns = hd::timeit([](){ sleep(1); });
    printf("sleep(1) took %lf s\n", ns / 1.0e9);
}