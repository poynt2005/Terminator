cc = gcc
cxx = g++
cflags = 
ldflags = 
libs = 

test:
	$(cxx) -c test.cc -o test.o
	$(cxx) -L./ test.o -o test.exe -lTerminater

dll:
	$(cxx) -fPIC -shared -std=c++17 -c Process.cc -o Process.o
	$(cxx) -fPIC -shared -DBUILDTERMINATERAPI -c Terminate.cc -o Terminate.o
	$(cxx) -shared Process.o Terminate.o -Wl,--out-implib,Terminater.lib -o  Terminater.dll
	-del Terminate.o Process.o
