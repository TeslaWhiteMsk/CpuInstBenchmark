#include <stdio.h>
#include <stdlib.h>
#include "cpudetection.h"
#include "x86base.h"

int main(){
	printf("CPU Instructions Benchmark 0.0.6 (amd64) by TeslaWhiteMsk\n");
	struct cpufeatretdata cpuinstset= cpudetectionmain();
	if (cpuinstset.x86==1){
		x86main();
	} 
	return 0;
}
