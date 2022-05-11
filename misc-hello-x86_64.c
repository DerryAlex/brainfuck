#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

unsigned char code[] = {
	0x55, /* push    %rbp */
	0x48, 0x89, 0xe5, /* mov    %rsp, %rbp */
	0x48, 0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00, /* mov    $0x1,%rax */
	0x48, 0xc7, 0xc7, 0x01, 0x00, 0x00, 0x00, /* mov    $0x1,%rdi */
	0x48, 0x8d, 0x35, 0x00, 0x00, 0x00, 0x00, /* lea    $0x????????(%rip),%rsi */
	0x48, 0xc7, 0xc2, 0x00, 0x00, 0x00, 0x00, /* mov    $0x????????,%rdx */
	0x0f, 0x05, /* syscall */
	0x48, 0x89, 0xec, /* mov    %rbp, %rsp */
	0x5d, /* pop    %rbp */
	0xc3 /* ret */
};

char string[] = "Hello,world!\n";

void *memory;

typedef void (*fn)(void);

int main(int argc, char *argv[])
{
	int page_size = sysconf(_SC_PAGESIZE);
	int buffer_size = ((sizeof(code) - 1) / page_size + 1) * page_size;
	if (posix_memalign(&memory, page_size, buffer_size) != 0)
	{
		printf("Insufficient memory.\n");
		return 0;
	}
	if (mprotect(memory, buffer_size, PROT_READ | PROT_WRITE) != 0)
	{
		printf("%s\n", strerror(errno));
		return 0;
	}
	memcpy(memory, code, sizeof(code));
	unsigned arg = (void *)string - (memory + 18) - 7;
	memcpy(memory + 21, &arg, sizeof(arg)); /* lea string(%rip), %rsi */
	arg = strlen(string);
	memcpy(memory + 28, &arg, sizeof(arg)); /* mov $13, %rdx */
	if (mprotect(memory, buffer_size, PROT_READ | PROT_EXEC) != 0)
	{
		printf("%s\n", strerror(errno));
		return 0;
	}
	fn func = memory;
	func();
	return 0;
}
