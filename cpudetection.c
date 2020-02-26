//CPU DETECTION MODULE

#include <stdio.h>
#include <stdlib.h>
#include "cpudetection.h"



int cpudetectionmain(){
	int cpuflag =cpuidcheck();
	if (cpuflag==1) {
		printf("CPUID is  suported!\n");
	}else {
		printf("CPUID is not suported! Maybe you using CPU lower than 486, lol?\n");
		exit (1);
	}
	char* vendorname= (char*)malloc(13*sizeof(char));
	if (vendorname==NULL){
		printf("Malloc error in vendorname!\n");
		exit (1);
	}
	vendorname= cpuvendor(vendorname);
	printf("CPU Vendor is : %s\n",vendorname);
	free (vendorname);
	int maxcpuid= maxcpuidval();
	printf("Basic max CPUID value = 0x%Xh\n", maxcpuid );
	if (maxcpuid>=0x1){
		printfammodstep();
		//printindxcores();

	}



	int exmaxcpuid=extmaxcpuid();
	printf("Extended max CPUID value = 0x%Xh\n", maxcpuid );
	if (exmaxcpuid>=0x80000004){
		printcpubrand();
	}
	printcpufeatures(maxcpuid,exmaxcpuid);

	return 0;
}

void printfammodstep(){
	struct EAX01H fmsreg=fammodstep();
	unsigned int type=fmsreg.type;
	unsigned int steping= fmsreg.steping;
	unsigned int model=(fmsreg.extmodel<<4) +fmsreg.model;
	unsigned int family=fmsreg.extfamily+fmsreg.family;
	printf("Steping = 0x%Xh;\tModel= %u\tFamily=%u\n",steping, model,family);
	printcpudescr(family,model,type);
	return;
}

struct EAX01H fammodstep(){
	asm(
		"pushq %rbx\n"
		"movq $1, %rax\n"
		"cpuid\n"
		"popq %rbx\n"
	);
}

void printcpubrand(){
	char* brandstring= (char*)malloc(49*sizeof(char));
	if (brandstring==NULL){
		printf("Malloc error in brandstring!\n");
		exit(1);
	}
	brandstring=brandstr(brandstring);
	printf("CPU Name is : %s\n",brandstring);
	free(brandstring);
	return;
}
char* brandstr (char* a){
	asm(
		"pushq %rbx\n"
		"movq %rdi,%rsi\n" //заранее сохраняем начальный адрес
		"movq $0x80000002, %rax\n"
		"cpuid\n"
		"call savestring\n"
		"movq $0x80000003, %rax\n"
		"cpuid\n"
		"call savestring\n"
		"movq $0x80000004, %rax\n"
		"cpuid\n"
		"call savestring\n"
		"movq $0, %rdi\n" //конечный ноль
		"movq %rsi, %rax\n"
		"jmp endop\n"


		"savestring:\n" //функция сохраняющая 16 байт  строки
		"mov %eax, (%rdi)\n"
		"addq $4,%rdi\n"
		"mov %ebx, (%rdi)\n"
		"addq $4,%rdi\n"
		"mov %ecx, (%rdi)\n"
		"addq $4,%rdi\n"
		"mov %edx, (%rdi)\n"
		"addq $4,%rdi\n"
		"retq\n"



		"endop:\n"
		"popq %rbx\n"

	);
}
int maxcpuidval (){
	asm(
		"pushq %rbx\n"
		"movq $0, %rax\n"
		"cpuid\n"
		"popq %rbx\n"
	);
}
char* cpuvendor(char* cpuvend){
	asm(
		"pushq %rbx\n"
		"movq $0, %rax\n"
		"cpuid\n"
		"movq %rdi, %rax \n"
		"mov %ebx,(%rdi)\n"
		"addq $4,%rdi\n"
		"mov %edx,(%rdi)\n"
		"addq $4,%rdi\n"
		"mov %ecx,(%rdi)\n"
		"addq $4,%rdi\n"
		"movq $0,(%rdi)\n"
		"popq %rbx\n"
	);
}

int extmaxcpuid (){
	asm(
		"pushq %rbx\n"
		"movq $0x80000000, %rax\n"
		"cpuid\n"
		"popq %rbx\n"
	);
}

int cpuidcheck(){
	asm(
		"pushq %rbx\n"
		"pushfq\n"
		"popq %rax\n"
		"movq %rax, %rbx\n"
		"xor $0x200000 ,%rax\n"
		"pushq %rax\n"
		"popfq\n"
		"pushfq\n"
		"popq %rax\n"
		"xor %rbx,%rax\n"
		"je nocpuid\n"
		"movq $1, %rax\n"
		"jmp endid\n"
		"nocpuid:\n"
		"movq $0, %rax\n"
		"endid:\n"
		"popq %rbx\n"
	);
}

void printindxcores(){
	struct EBX01H indxcores = getindxcores();
	printf("Brand index = 0x%Xh\tLogical cores = %i\n",indxcores.brand_id,indxcores.cores_count );
	printf("APIC ID = 0x%Xh\n",indxcores.local_apic_id);
}

struct EBX01H getindxcores(){
	asm(
		"pushq %rbx\n"
		"movq $1, %rax\n"
		"cpuid\n"
		"movq %rbx, %rax\n"
		"popq %rbx\n"
	);
}

void printcpufeatures (int maxcpuid,int exmaxcpuid){
	struct ECX01H cpufeat1;
	struct EDX01H cpufeat2;
	struct EBX07H_00H cpufeat3;
	struct ECX07H_00H cpufeat4;
	struct EDX07H_00H cpufeat5;
	struct EAX07H_01H cpufeat6;
	struct EDX801H cpufeat7;
	struct ECX801H cpufeat8;
	if (maxcpuid>=1){
		cpufeat1=getcpufeatures1();
		cpufeat2=getcpufeatures2();
	}
	if (maxcpuid>=7){
		cpufeat3=getcpufeatures3();
		cpufeat4=getcpufeatures4();
		cpufeat5=getcpufeatures5();
		cpufeat6=getcpufeatures6();
	}
	if (exmaxcpuid>=0x80000001){
		cpufeat7=getcpufeatures7();
		cpufeat8=getcpufeatures8();
	}


	printf("-------------------------------------------\n");
	printf("Intel Instructions set supporting:\n");
	if (maxcpuid>=1){
		printf("FPU X87\t\t|%i\n",cpufeat2.fpu );
		printf("MMX\t\t|%i\n",cpufeat2.mmx );
		printf("SSE\t\t|%i\n",cpufeat2.sse );
		printf("SSE2\t\t|%i\n",cpufeat2.sse2 );
		printf("SSE3\t\t|%i\n",cpufeat1.sse3 );
		printf("SSSE3\t\t|%i\n",cpufeat1.sse3_sup );
		printf("SSE4.1\t\t|%i\n",cpufeat1.sse41 );
		printf("SSE4.2\t\t|%i\n",cpufeat1.sse42 );
		printf("F16C\t\t|%i\n",cpufeat1.f16c );
		printf("FMA\t\t|%i\n",cpufeat1.fma );
		printf("AVX\t\t|%i\n",cpufeat1.avx );
	}
	if (maxcpuid>=7){
		printf("AVX2\t\t|%i\n",cpufeat3.avx2 );
		printf("AVX512_FOUNDAT\t|%i\n",cpufeat3.avx512_f );
		printf("AVX512_DWQWINST\t|%i\n",cpufeat3.avx512_dq );
		printf("AVX512_INT_FMA\t|%i\n",cpufeat3.avx512_ifma );
		printf("AVX512_PREFETCH\t|%i\n",cpufeat3.avx512_pf );
		printf("AVX512_EXP_RECP\t|%i\n",cpufeat3.avx512_er );
		printf("AVX512_CONF_DET\t|%i\n",cpufeat3.avx512_cd );
		printf("AVX512_BYTE_WORD|%i\n",cpufeat3.avx512_bw );
		printf("AVX512_VECTOR_LE|%i\n",cpufeat3.avx512_vl );
		printf("AVX512_VECT_BMI\t|%i\n",cpufeat4.avx512_vbmi);
		printf("AVX512_VECT_BMI2|%i\n",cpufeat4.avx512_vbmi2);
		printf("AVX512_VECT_NNI\t|%i\n",cpufeat4.avx512_vnni);
		printf("AVX512_BITALG\t|%i\n",cpufeat4.avx512_bitalg);
		printf("AVX512_VPOPCNTDQ|%i\n",cpufeat4.avx512_vpopcntdq);
		printf("AVX512_4VECT_NNI|%i\n",cpufeat5.avx512_4vnniw);
		printf("AVX512_4FMAPS\t|%i\n",cpufeat5.avx512_4fmaps);
		printf("AVX512_BFLOAT16\t|%i\n",cpufeat6.avx512_bf16);

		printf("BMI1\t\t|%i\n",cpufeat3.bmi1 );
		printf("BMI2\t\t|%i\n",cpufeat3.bmi2 );
		printf("ADX\t\t|%i\n",cpufeat3.adx);
	}
	printf("-------------------------------------------\n");
	printf("AMD Instructions set supporting:\n");
	if (exmaxcpuid>=0x80000001){
		printf("MMX_EXT\t\t|%i\n",cpufeat7.mmxext );
		printf("3DNOW!\t\t|%i\n",cpufeat7._3dnow );
		printf("EXT_3DNOW!\t|%i\n",cpufeat7._3dnowext );
		printf("XOP\t\t|%i\n",cpufeat8.xop );
		printf("TBM\t\t|%i\n",cpufeat8.tbm );
		printf("FMA4\t\t|%i\n",cpufeat8.fma4 );
		printf("SSE4A\t\t|%i\n",cpufeat8.sse4a );
	}
	printf("-------------------------------------------\n");
	printf("Intel single instructions supporting:\n");
	if (maxcpuid>=1){
		printf("CMOV\t\t|%i\n",cpufeat2.cmov );
		printf("CLFLUSH\t\t|%i\n",cpufeat2.clfsh );
		printf("XSAVE\t\t|%i\n",cpufeat1.xsave );
		printf("OSXSAVE\t\t|%i\n",cpufeat1.osxsave );
		printf("FXSR\t\t|%i\n",cpufeat2.fxsr );
		printf("MOVBE\t\t|%i\n",cpufeat1.movbe );
		printf("POPCNT\t\t|%i\n",cpufeat1.popcnt);

	}
	if (maxcpuid>=7){
		printf("ERMS\t\t|%i\n",cpufeat3.erms);
		printf("INVPCID\t\t|%i\n",cpufeat3.invpcid);
		printf("CLWB\t\t|%i\n",cpufeat3.clwb);
		printf("PCOMMIT\t\t|%i\n",cpufeat3.pcommit);
		printf("CLFLUSHOPT\t|%i\n",cpufeat3.cflushopt);
	}
	printf("-------------------------------------------\n");
	printf("CMPXCHG Instructions supporting:\n");
	if (maxcpuid>=1){
		printf("CX8\t\t|%i\n",cpufeat2.cx8);
		printf("CX16\t\t|%i\n",cpufeat1.cx16 );
	}
	printf("-------------------------------------------\n");
	printf("Cryptographic Instructions set supporting:\n");
	if (maxcpuid>=1){
		printf("AES\t\t|%i\n",cpufeat1.aes );
		printf("CLMUL\t\t|%i\n",cpufeat1.pclmulqdq );
		printf("RDRAND\t\t|%i\n",cpufeat1.rdrand);
	}
	if (maxcpuid>=7){
		printf("VAES\t\t|%i\n",cpufeat4.vaes );
		printf("VCLMUL\t\t|%i\n",cpufeat4.vpclmulqdq );
		printf("RDSEED\t\t|%i\n",cpufeat3.rdseed);
		printf("SHA\t\t|%i\n",cpufeat3.sha);
	}
	printf("-------------------------------------------\n");
	printf("Energy saving Instructions set supporting:\n");
	if (maxcpuid>=1){
		printf("MONITOR|MWAIT\t|%i\n",cpufeat1.monitor );
	}
	printf("-------------------------------------------\n");
	printf("Debug Instructions set supporting:\n");
	if (maxcpuid>=1){
		printf("DE\t\t|%i\n",cpufeat2.de);
		printf("DS\t\t|%i\n",cpufeat2.ds);
		printf("DTES64\t\t|%i\n",cpufeat1.dtes64);
		printf("DS-CPL\t\t|%i\n",cpufeat1.ds_cpl);
	}
	if (maxcpuid>=7){
		printf("INTEL-PT\t|%i\n",cpufeat3.intel_pt);
	}
	printf("-------------------------------------------\n");
	printf("Thermal control supporting:\n");
	if (maxcpuid>=1){
		printf("TM\t\t|%i\n",cpufeat2.tm);
		printf("TM2\t\t|%i\n",cpufeat1.tm2);
	}
	printf("-------------------------------------------\n");
	printf("Virtualization Instructions set supporting:\n");
	if (maxcpuid>=1){
		printf("VME\t\t|%i\n",cpufeat2.vme);
		printf("VMX\t\t|%i\n",cpufeat1.vmx);
	}
	printf("-------------------------------------------\n");
	printf("Memory Management supporting:\n");
	if (maxcpuid>=1){
		printf("PAE\t\t|%i\n",cpufeat2.pae);
		printf("MTRR\t\t|%i\n",cpufeat2.mtrr);
		printf("PGE\t\t|%i\n",cpufeat2.pge);
		printf("PAT\t\t|%i\n",cpufeat2.pat);
		printf("PSE\t\t|%i\n",cpufeat2.pse);
		printf("PSE-36\t\t|%i\n",cpufeat2.pse36);
	}
	if (maxcpuid>=7){
		printf("TSX-HWL_EXT\t|%i\n",cpufeat3.hle);
		printf("TSX-RTM\t\t|%i\n",cpufeat3.rtm);
	}
	if (exmaxcpuid>=0x80000001){
		printf("1GB_PAGE\t|%i\n",cpufeat7.pdpe1gb );
	}
	printf("-------------------------------------------\n");
	printf("Timestamp Instructions set supporting:\n");
	if (maxcpuid>=1){
		printf("TSC\t\t|%i\n",cpufeat2.tsc);
		printf("TSCD\t\t|%i\n",cpufeat1.tscdeadline);
	}
	printf("-------------------------------------------\n");
	printf("Other supporting:\n");
	if (maxcpuid>=1){
		printf("MSR\t\t|%i\n",cpufeat2.msr);
		printf("MCA\t\t|%i\n",cpufeat2.mca);
		printf("MCE\t\t|%i\n",cpufeat2.mce);
		printf("APIC\t\t|%i\n",cpufeat2.apic);
		printf("x2APIC\t\t|%i\n",cpufeat1.x2apic);
		printf("SYSCALL|SYSRET\t|%i\n",cpufeat2.sep);
		printf("PSN\t\t|%i\n",cpufeat2.psn);
		printf("ACPI\t\t|%i\n",cpufeat2.acpi);
		printf("SS\t\t|%i\n",cpufeat2.ss);
		printf("HTT\t\t|%i\n",cpufeat2.htt);
		printf("PBE\t\t|%i\n",cpufeat2.pbe);
		printf("SMX\t\t|%i\n",cpufeat1.smx);
		printf("EIST\t\t|%i\n",cpufeat1.eist);
		printf("CNXT-ID\t\t|%i\n",cpufeat1.cnxt_id);
		printf("xTPR\t\t|%i\n",cpufeat1.xtpr);
		printf("PDCM\t\t|%i\n",cpufeat1.pdcm);
		printf("PCID\t\t|%i\n",cpufeat1.pcid);
		printf("DCA\t\t|%i\n",cpufeat1.dca);
		printf("Hypervisor mode\t|%i\n",cpufeat1.raz );
	}
	if (maxcpuid>=7){
		printf("UMIP\t\t|%i\n",cpufeat4.umip);
		printf("SMAP\t\t|%i\n",cpufeat3.smap);
		printf("SMEP\t\t|%i\n",cpufeat3.smep);
		printf("PQE\t\t|%i\n",cpufeat3.pqe);
		printf("PQM\t\t|%i\n",cpufeat3.pqm);
		printf("PREFETCHWT1\t|%i\n",cpufeat4.prefetchwt1);
	}
	printf("-------------------------------------------\n");
}

struct ECX01H getcpufeatures1(){
	asm(
		"pushq %rbx\n"
		"movq $1, %rax\n"
		"cpuid\n"
		"movq %rcx, %rax\n"
		"popq %rbx\n"
	);
}

struct EDX01H getcpufeatures2(){
	asm(
		"pushq %rbx\n"
		"movq $1, %rax\n"
		"cpuid\n"
		"movq %rdx, %rax\n"
		"popq %rbx\n"
	);
}

struct EBX07H_00H getcpufeatures3(){
	asm(
		"pushq %rbx\n"
		"movq $7, %rax\n"
		"movq $0, %rcx\n"
		"cpuid\n"
		"movq %rbx, %rax\n"
		"popq %rbx\n"
	);
}

struct ECX07H_00H getcpufeatures4(){
	asm(
		"pushq %rbx\n"
		"movq $7, %rax\n"
		"movq $0, %rcx\n"
		"cpuid\n"
		"movq %rcx, %rax\n"
		"popq %rbx\n"
	);
}

struct EDX07H_00H getcpufeatures5(){
	asm(
		"pushq %rbx\n"
		"movq $7, %rax\n"
		"movq $0, %rcx\n"
		"cpuid\n"
		"movq %rdx, %rax\n"
		"popq %rbx\n"
	);
}

struct EAX07H_01H getcpufeatures6(){
	asm(
		"pushq %rbx\n"
		"movq $7, %rax\n"
		"movq $1, %rcx\n"
		"cpuid\n"
		"popq %rbx\n"
	);
}

struct EDX801H getcpufeatures7(){
	asm(
		"pushq %rbx\n"
		"movq $0x80000001, %rax\n"
		"cpuid\n"
		"movq %rdx, %rax\n"
		"popq %rbx\n"
	);
}

struct ECX801H getcpufeatures8(){
	asm(
		"pushq %rbx\n"
		"movq $0x80000001, %rax\n"
		"cpuid\n"
		"movq %rcx, %rax\n"
		"popq %rbx\n"
	);
}

void printcpudescr(int fam,int mod, int type ){
	switch (fam) {
		case 3:
		switch (type) {
			case 0:
			printf("Intel386 DX\n");
			break;
			case 2:
			printf("Intel386 SX/CX/EX\n");
			break;
			case 4:
			printf("Intel386 SL\n");
			break;
			default:
			printf("Intel386-like\n");
			break;
		}
		break;
		case 4:
		switch (mod) {
			case 1:
			printf("Intel486 DX\n");
			break;
			case 2:
			printf("Intel486 SX\n");
			break;
			case 3:
			printf("Intel487/DX2/DX2 OverDrive\n");
			break;
			case 4:
			printf("Intel486 SL\n");
			break;
			case 5:
			printf("IntelSX2\n");
			break;
			case 7:
			printf("IntelDX2 Write-Back Enhanced\n");
			break;
			case 8:
			printf("Intel/DX4/DX4 OverDrive\n");
			break;
			default:
			printf("Intel486-like\n");
			break;




		}
		break;
		default:
		break;
	}
}
