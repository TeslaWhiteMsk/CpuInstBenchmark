binary: cpudetection.o main.o
	gcc -o binary cpudetection.o main.o
cpudetection.o: cpudetection.c
	gcc -c cpudetection.c
main.o: main.c
	gcc -c main.c
clean:
	rm -f *.o binary
