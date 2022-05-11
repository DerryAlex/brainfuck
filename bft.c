#include "bft.h"

void translate(char *code)
{
	FILE *f;
	if ((f = fopen("/tmp/brainfuck_translate.c", "w")) == NULL)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		return;
	}
	fprintf(f, "#include <stdio.h>\n");
	fprintf(f, "char cells[%d];\n", MEMORY_SIZE);
	fprintf(f, "char *ptr = cells;\n");
	fprintf(f, "int main() {\n");
	for (int i = 0; code[i] != '\0'; i++)
	{
		switch (code[i]) {
			case '>': fprintf(f, "ptr++;\n");
				  break;
			case '<': fprintf(f, "ptr--;\n");
				  break;
			case '+': fprintf(f, "(*ptr)++;\n");
				  break;
			case '-': fprintf(f, "(*ptr)--;\n");
				  break;
			case '.': fprintf(f, "putchar(*ptr);\n");
				  break;
			case ',': fprintf(f, "*ptr = getchar();\n");
				  break;
			case '[': fprintf(f, "while (*ptr) {\n");
				  break;
			case ']': fprintf(f, "}\n");
				  break;
			default: break;
		}
	}
	fprintf(f, "}\n");
	fclose(f);
	system("gcc /tmp/brainfuck_translate.c -O2");
	system("rm /tmp/brainfuck_translate.c");
	return;
}
