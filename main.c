#include <stdio.h>
#include <stdlib.h>
#include "cpudetection.h"

int main(){
	printf("CPU Instructions Benchmark 0.0.6 (amd64) by TeslaWhiteMsk\n");
	struct cpufeatretdata cpuinstset= cpudetectionmain();

	return 0;
}
