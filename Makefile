.c.o:   $*.h
	gcc -c $*.c -I./external/SOIL/inc

.cpp.o:	$*.h
	g++	-c $*.cpp -I./external/SOIL/inc

all:	arvore

arvore:  arvore.o winGL.o 
	g++ -o $@ $^ -L./external/SOIL/lib -lglu32 -lSOIL -lopengl32 -L"D:\msys64\mingw64\lib" -lfreeglut

clean:
	rm *.o *.*~ *~ arvore