#include <stdio.h>

void main()
{
#if defined(VAR)
	printf("var is %d\n", VAR);
#else
	printf("no var\n");
#endif
}
