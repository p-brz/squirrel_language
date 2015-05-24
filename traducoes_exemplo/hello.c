#include "sqlib.h"

int main ()
{
    squirrel_setup();
    printf("%s\n", create_String("Hello World").cstr);
    free_memory();
	return 0;
}
