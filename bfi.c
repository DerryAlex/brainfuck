#include "bfi.h"

static int pc = 0;
static char memory[MEMORY_SIZE] = {0};
static int memory_pointer = 0;
static int stack[STACK_SIZE] = {0};
static int stack_top = 0;
static int map[CODE_SIZE] = {0};

static int excute(char *ir)
{
	if (memory_pointer < 0 || memory_pointer >= MEMORY_SIZE)
		goto out_of_bound;
	switch (ir[pc]) {
		case '>': memory_pointer++;
			  break;
		case '<': memory_pointer--;
			  break;
		case '+': memory[memory_pointer]++;
			  break;
		case '-': memory[memory_pointer]--;
			  break;
		case '.': putchar(memory[memory_pointer]);
			  break;
		case ',': memory[memory_pointer] = getchar();
			  break;
		case '[': if (memory[memory_pointer] == 0) pc = map[pc];
			  break;
		case ']': if (memory[memory_pointer] != 0) pc = map[pc];
			  break;
		case '\0': return EXECUTE_DONE;
		default: break;
	}
	pc++;
	return EXECUTE_SUCCESS;
out_of_bound:
	printf("Memory: Out of Bound.\n");
	return EXECUTE_FAILURE;
}

void interpret(char *code)
{
	for (int i = 0; code[i] != '\0'; i++) {
		if (code[i] == '[') stack[stack_top++] = i;
		if (code[i] == ']') {
			if (stack_top == 0) goto unmatched;
			stack_top--;
			map[i] = stack[stack_top];
			map[stack[stack_top]] = i;
		}
	}
	if (stack_top != 0) goto unmatched;
	while (excute(code) == EXECUTE_SUCCESS);
	return;
unmatched:
	printf("Bracket: Unmatched.\n");
	return;
}
