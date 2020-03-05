#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "x86base.h"

void x86main(){
	printf("-------------------------------------------\n");
	printf("X86 INSTRUCTIONS TESTING\n");
	printf("TEST 1: Matrix addition\n");
	x86_test1();
	printf("TEST 2: Matrix subtraction\n");
	x86_test2();
};

void x86_test1(){
	clock_t start;
	clock_t end;
	double resulttime;
	int* data1=(int*)malloc(4000000*sizeof(int));
	int* data2=(int*)malloc(4000000*sizeof(int));
	int* resultdata=(int*)malloc(4000000*sizeof(int));
	if(data1==NULL||data2==NULL||resultdata==NULL){
		printf("MALLOC ERROR IN X86BASE MODULE!!! EXIT...\n");
		exit (1);
	}
	printf("Generating set for test 1...\n");
	for (int i = 0; i < 4000000 ; i++) {
		data1[i]= (-i)*2;
		data2[i]= i*3;
	}
	start=clock();
	x86_test1_asm(data1,data2,resultdata);
	end=clock();
	resulttime=((double)end-start)/CLOCKS_PER_SEC;
	printf("Result time is =%f sec \n",resulttime );
	free(data1);
	free(data2);
	free(resultdata);
}

void x86_test1_asm(int* a, int* b, int*c){
	asm(
		"pushq %rbx\n"
		"movq $3999999, %rcx\n"

		"x86operation1:\n"
		"dec %rcx\n"
		"je x86end1\n"
		"mov (%rdi),%rax\n"
		"add (%rsi),%rax\n"
		"mov %rax,(%rdx)\n"
		"inc %rdi\n"
		"inc %rsi\n"
		"inc %rdx\n"
		"jmp x86operation1\n"

		"x86end1:\n"
		"popq %rbx\n"
	);
}

void x86_test2(){
	clock_t start;
	clock_t end;
	double resulttime;
	int* data1=(int*)malloc(4000000*sizeof(int));
	int* data2=(int*)malloc(4000000*sizeof(int));
	int* resultdata=(int*)malloc(4000000*sizeof(int));
	if(data1==NULL||data2==NULL||resultdata==NULL){
		printf("MALLOC ERROR IN X86BASE MODULE!!! EXIT...\n");
		exit (1);
	}
	printf("Generating set for test 2...\n");
	for (int i = 0; i < 4000000 ; i++) {
		data1[i]= i*3;
		data2[i]= i*2;
	}
	start=clock();
	x86_test2_asm(data1,data2,resultdata);
	end=clock();
	resulttime=((double)end-start)/CLOCKS_PER_SEC;
	printf("Result time is =%f sec \n",resulttime );
	free(data1);
	free(data2);
	free(resultdata);
}

void x86_test2_asm(int* a, int* b, int*c){
	asm(
		"pushq %rbx\n"
		"movq $3999999, %rcx\n"

		"x86operation2:\n"
		"dec %rcx\n"
		"je x86end2\n"
		"mov (%rdi),%rax\n"
		"sub (%rsi),%rax\n"
		"mov %rax,(%rdx)\n"
		"inc %rdi\n"
		"inc %rsi\n"
		"inc %rdx\n"
		"jmp x86operation2\n"

		"x86end2:"
		"popq %rbx\n"
	);
}
