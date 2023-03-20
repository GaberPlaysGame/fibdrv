#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
// #include <bits/types/struct_timespec.h>

#define FIB_DEV "/dev/fibonacci"

static inline long long clock_gettime_ns()
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    return t.tv_sec * 1000000000 + t.tv_nsec;
}

int main()
{
    // uint8_t sz;

    char buf[128];
    char write_buf[] = "testing writing";
    int offset = 500; /* TODO: try test something bigger than the limit */

    int fd = open(FIB_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        exit(1);
    }

    // for (int i = 0; i <= offset; i++) {
    //     sz = write(fd, write_buf, strlen(write_buf));
    //     printf("Writing to " FIB_DEV ", returned the sequence %lld\n", sz);
    // }

    // for (int i = 0; i <= offset; i++) {
    //     lseek(fd, i, SEEK_SET);
    //     sz = read(fd, buf, sizeof(buf));
    //     buf[sz] = 0;
    //     printf("Reading from " FIB_DEV
    //            " at offset %d, returned the sequence "
    //            "%s.\n",
    //            i, buf);
    // }

    // for (int i = offset; i >= 0; i--) {
    //     lseek(fd, i, SEEK_SET);
    //     sz = read(fd, buf, sizeof(buf));
    //     buf[sz] = 0;
    //     printf("Reading from " FIB_DEV
    //            " at offset %d, returned the sequence "
    //            "%s.\n",
    //            i, buf);
    // }

    for (int i = 0; i <= offset; i++) {
        lseek(fd, i, SEEK_SET);
        long long temp = clock_gettime_ns();
        buf[read(fd, buf, sizeof(buf))] = 0;
        long long utime = clock_gettime_ns() - temp;

        // printf("Reading from " FIB_DEV
        //        " at offset %d, returned the sequence "
        //        "%s.\n",
        //        i, buf);
        long long ktime = write(fd, write_buf, sizeof(write_buf));
        long long kutime = utime - ktime;
        // printf("The kernel time from fib function is %lld.\n", kt);
        printf("%d %lld %lld %lld\n", i, utime, ktime, kutime);
    }

    close(fd);
    return 0;
}
