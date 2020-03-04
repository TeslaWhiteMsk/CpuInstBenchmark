#ifndef CPUDETECTION_H
#define CPUDETECTION_H


struct EAX01H{
	unsigned int steping:4;
	unsigned int model:4;
	unsigned int family:4;
	unsigned int type:2;
	unsigned :2;
	unsigned int extmodel:4;
	unsigned int extfamily :8;
	unsigned :4;

};

struct EBX01H{
	unsigned int brand_id:8;
	unsigned int clfush_size:8;
	unsigned int cores_count:8;
	unsigned int local_apic_id:8;
};

struct ECX01H{
	unsigned int sse3:1;
	unsigned int pclmulqdq:1;
	unsigned int dtes64:1;
	unsigned int monitor:1;
	unsigned int ds_cpl:1;
	unsigned int vmx:1;
	unsigned int smx:1;
	unsigned int eist:1;
	unsigned int tm2:1;
	unsigned int sse3_sup:1;
	unsigned int cnxt_id:1;
	unsigned:1;
	unsigned int fma:1;
	unsigned int cx16:1;
	unsigned int xtpr:1;
	unsigned int pdcm:1;
	unsigned:1;
	unsigned int pcid:1;
	unsigned int dca:1;
	unsigned int sse41:1;
	unsigned int sse42:1;
	unsigned int x2apic:1;
	unsigned int movbe:1;
	unsigned int popcnt:1;
	unsigned int tscdeadline:1;
	unsigned int aes:1;
	unsigned int xsave:1;
	unsigned int osxsave:1;
	unsigned int avx:1;
	unsigned int f16c:1;
	unsigned int rdrand:1;
	unsigned int raz:1;
};

struct EDX01H {
	unsigned int fpu:1;
	unsigned int vme:1;
	unsigned int de:1;
	unsigned int pse:1;
	unsigned int tsc:1;
	unsigned int msr:1;
	unsigned int pae:1;
	unsigned int mce:1;
	unsigned int cx8:1;
	unsigned int apic:1;
	unsigned:1;
	unsigned int sep:1;
	unsigned int mtrr:1;
	unsigned int pge:1;
	unsigned int mca:1;
	unsigned int cmov:1;
	unsigned int pat:1;
	unsigned int pse36:1;//
	unsigned int psn:1;
	unsigned int clfsh:1;
	unsigned:1;
	unsigned int ds:1;
	unsigned int acpi:1;
	unsigned int mmx:1;
	unsigned int fxsr:1;
	unsigned int sse:1;
	unsigned int sse2:1;
	unsigned int ss:1;
	unsigned int htt:1;
	unsigned int tm:1;
	unsigned:1;
	unsigned int pbe:1;

};

struct EBX07H_00H{
	unsigned int fsgsbase:1;
	unsigned int IA32_TSC_ADJUST:1;
	unsigned int sgx:1;
	unsigned int bmi1:1;
	unsigned int hle:1;
	unsigned int avx2:1;
	unsigned :1;
	unsigned int smep:1;
	unsigned int bmi2:1;
	unsigned int erms:1;
	unsigned int invpcid:1;
	unsigned int rtm:1;
	unsigned int pqm:1;
	unsigned int fpucs_ds_dep:1;
	unsigned int mpx:1;
	unsigned int pqe:1;
	unsigned int avx512_f:1;
	unsigned int avx512_dq:1;
	unsigned int rdseed:1;
	unsigned int adx:1;
	unsigned int smap:1;
	unsigned int avx512_ifma:1;
	unsigned int pcommit:1;
	unsigned int cflushopt:1;
	unsigned int clwb:1;
	unsigned int intel_pt:1;
	unsigned int avx512_pf:1;
	unsigned int avx512_er:1;
	unsigned int avx512_cd:1;
	unsigned int sha:1;
	unsigned int avx512_bw:1;
	unsigned int avx512_vl:1;
};

struct ECX07H_00H {
	unsigned int prefetchwt1:1;
	unsigned int avx512_vbmi:1;
	unsigned int umip:1;
	unsigned int pku:1;
	unsigned int ospke:1;
	unsigned int waitpkg:1;
	unsigned int avx512_vbmi2:1;
	unsigned int shstk:1;
	unsigned int gfni:1;
	unsigned int vaes:1;
	unsigned int vpclmulqdq:1;
	unsigned int avx512_vnni:1;
	unsigned int avx512_bitalg:1;
	unsigned:1;
	unsigned int avx512_vpopcntdq:1;
	unsigned:1;
	unsigned int fivelevelpaging:1;
	unsigned int mawau:5;
	unsigned int rdpid:1;
	unsigned:2;
	unsigned int cldmote:1;
	unsigned:1;
	unsigned int movdir:1;
	unsigned int movdir64b:1;
	unsigned:1;
	unsigned int sgx_lc:1;
	unsigned:1;
};

struct EDX07H_00H{
	unsigned:2;
	unsigned int avx512_4vnniw:1;
	unsigned int avx512_4fmaps:1;
	unsigned int fsrm:1;
	unsigned:13;
	unsigned int pconfig:1;
	unsigned:1;
	unsigned int ibt:1;
	unsigned:5;
	unsigned int spec_ctrl:1;
	unsigned int stibp:1;
	unsigned:1;
	unsigned int capabilities:1;
	unsigned:1;
	unsigned int ssbd:1;
};

struct EAX07H_01H{
	unsigned:5;
	unsigned int avx512_bf16:1;
	unsigned:26;
};

struct EDX801H{
	unsigned:19;
	unsigned int mp:1;
	unsigned int nx:1;
	unsigned:1;
	unsigned int mmxext:1;
	unsigned:2;
	unsigned int fxsr_opt:1;
	unsigned int pdpe1gb:1;
	unsigned int rdtscp:1;
	unsigned:1;
	unsigned int lm:1;
	unsigned int _3dnowext:1;
	unsigned int _3dnow:1;
};

struct ECX801H{
	unsigned int lahf_lm:1;
	unsigned int cmp_legacy:1;
	unsigned int svm:1;
	unsigned int extapic:1;
	unsigned int cr8_legacy:1;
	unsigned int abm:1;
	unsigned int sse4a:1;
	unsigned int misalignsse:1;
	unsigned int _3dnowprefetch:1;
	unsigned int osvw:1;
	unsigned int ibs:1;
	unsigned int xop:1;
	unsigned int skinit:1;
	unsigned int wdt:1;
	unsigned:1;
	unsigned int lwp:1;
	unsigned int fma4:1;
	unsigned int tce:1;
	unsigned:1;
	unsigned int nodeid_msr:1;
	unsigned:1;
	unsigned int tbm:1;
	unsigned int topoext:1;
	unsigned int perfctr_core:1;
	unsigned int perfctr_nb:1;
	unsigned:1;
	unsigned int dbx:1;
	unsigned int perfitsc:1;
	unsigned int pcx_i2i:1;
	unsigned:3;
};

int cpuidcheck(); //(asm)проверка поддержки CPUID (нужна для удобства портирования, в amd64 бесполезна)
char* cpuvendor(char* cpuvend); //(asm) Вывод названия производителя процессора
int maxcpuidval ();// (asm)  Максимальное значение базовой функции CPUID
int extmaxcpuid ();//(asm)  Максимальное значение расширенной функции CPUID
void printcpubrand();// Печать строки с названием процессора
void printfammodstep();//Печать модели, семейства, и степпинга процессора
char* brandstr (char* a);//(asm) Генерация строки с названием процессора
struct EAX01H fammodstep();//(asm)Генерация структуры с моделью семейством и степпингом проца
void printindxcores(); //Печать индекса, числа ядер и прочих параметров (пока что не работает как надо)
struct EBX01H getindxcores();//(asm) Генерация структуры с индексом, числом ядер и прочими параметрами
void printcpufeatures (int maxcpuid,int exmaxcpuid); //Печать возможностей процессора
struct ECX01H getcpufeatures1();//(asm) Генерация первой части битового поля CPU Features
struct EDX01H getcpufeatures2();//(asm) Генерация второй части битового поля CPU Features
struct EBX07H_00H getcpufeatures3();
struct ECX07H_00H getcpufeatures4();
struct EDX07H_00H getcpufeatures5();
struct EAX07H_01H getcpufeatures6();
struct EDX801H getcpufeatures7();
struct ECX801H getcpufeatures8();
int cpudetectionmain();
#endif /* end of include guard:  */
