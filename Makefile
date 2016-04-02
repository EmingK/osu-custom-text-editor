default: customrunner

customrunner: customrunner.o
	gcc -o customrunner.exe customrunner.o -nostdlib -nodefaultlibs -Wl,-subsystem,windows -lkernel32 -luser32

customrunner.o: customrunner.c
	gcc -O2 customrunner.c -o customrunner.o -c

clean:
	del *.o
	del *.exe
