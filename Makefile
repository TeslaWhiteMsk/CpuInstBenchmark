binary: cpudetection.o main.o x86base.o
	gcc -o binary cpudetection.o main.o x86base.o
cpudetection.o: cpudetection.c
	gcc -c cpudetection.c
x86base.o: x86base.c
	gcc -c x86base.c
main.o: main.c
	gcc -c main.c
clean:
	rm -f *.o binary
