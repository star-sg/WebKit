#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void db(uint32_t t, uint32_t p, uint32_t n, uint32_t k, uint32_t* a, FILE* fp) {
    if (t > n) {
        if (n % p == 0) {
            for (size_t i = 1; i <= p; ++i) {
                unsigned char c = a[i];
                fwrite(&c, 1, 1, fp);
            }
        }
    } else {
        a[t] = a[t - p];
        db(t + 1, p, n, k, a, fp);
        for (size_t i = a[t - p] + 1; i < k; ++i) {
            a[t] = i;
            db(t + 1, t, n, k, a, fp);
        }
    }
}

void de_bruijn(uint32_t k, uint32_t n, FILE *fp) {
    uint32_t* a = malloc(sizeof(uint32_t) * k * n);
    memset(a, 0, sizeof(uint32_t) * k * n);
    db(1, 1, n, k, a, fp);
    free(a);
}

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3 && argc != 4) {
        printf("Usage: %s [<filename> | <filename> <pattern>|<position>]\n", argv[0]);
        return -1;
    }

    if (argc == 2) {
        FILE *fp = fopen(argv[1], "wb");
        if (fp == NULL) {
            perror("fopen");
            return -1;
        }

        de_bruijn(2, 4, fp);
        fclose(fp);
    } else if (argc == 3) {
        FILE *fp = fopen(argv[1], "rb");
        if (fp == NULL) {
            perror("fopen");
            return -1;
        }
        size_t buf_size = 0x200000;
        unsigned char* buf = malloc(buf_size);
        unsigned found = 0;
        char* endptr;
        unsigned int inp = strtol(argv[2], &endptr, 10);
        for (size_t i=0; i < 1023; ++i) {
            size_t numread = fread(buf, 1, buf_size, fp);
            if (numread == 0) break;
            for (size_t j=0; j < numread; j+=4) {
                unsigned int tmp = *(unsigned int *)((char *)buf + j);
                if (inp == tmp) {
                    printf("%lu", i * buf_size + j);
                    found = 1;
                    break;
                }
            }
            if (found == 1) break;           
        }
        if (found == 0) printf("-1");
        free(buf);
    } else {
        FILE *fp = fopen(argv[1], "rb");
        if (fp == NULL) {
            perror("fopen");
            return -1;
        }
        size_t buf_size = 0x200000;
        unsigned char* buf = malloc(buf_size);
        unsigned found = 0;
        char* endptr;
        unsigned int inp = strtol(argv[2], &endptr, 10);
        for (size_t i=0; i < 1023; ++i) {
            size_t numread = fread(buf, 1, buf_size, fp);
            if (numread == 0) break;
            if (i * buf_size + numread > inp) {
                unsigned int tmp = *(unsigned int *)((char *)buf + inp - i * buf_size);
                for (size_t j=0; j < 4; ++j)
                    printf("%d ", *(unsigned char *)((char *)&tmp + j));
                break;
            }
        }
        free(buf);
    }
    
    return 0;
}
