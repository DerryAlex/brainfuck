#include "bf.h"
#include "bfi.h"
#include "bft.h"

static char code[CODE_SIZE + 16];

void load(char *filename)
{
	FILE *f;
	int bytes;
	if ((f = fopen(filename, "r")) == NULL) goto err;
	if ((bytes = fread(code, 1, CODE_SIZE + 16, f)) == 0)
		if (ferror(f)) goto err;
	if (bytes >= CODE_SIZE) goto err;
	fclose(f);
	return;
err:
	printf("%s\n", strerror(errno));
	exit(0);
}

int main(int argc, char *argv[])
{
	if (argc == 2) {
		load(argv[1]);
		translate(code);
		return 0;
	}
	else if (argc == 3) {
		if (strcmp(argv[1], "-i") == 0)
		{
			load(argv[2]);
			interpret(code);
			return 0;
		}
	}
helper:
	printf("Usage: bf [-i] program_file");
	return 0;
}
