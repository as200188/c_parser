#include <stdio.h>
#include "scanner/my_scanner.h"


int main()
{
	scanner sc("test.c");
	sc.printTokens();
	
	return 0;
}