CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g
OBJS = main.o archivo.o version.o

all: main.exe

main.exe: $(OBJS)
	$(CXX) -o main.exe $(OBJS)

todo: all

run: main.exe
	./main.exe

main.o: main.cpp archivo.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

archivo.o: archivo.cpp archivo.hpp version.hpp texto.hpp linea.hpp definiciones.hpp
	$(CXX) $(CXXFLAGS) -c archivo.cpp

version.o: version.cpp version.hpp texto.hpp linea.hpp
	$(CXX) $(CXXFLAGS) -c version.cpp

clean:
	del -f *.o main.exe 2>nul || true

info:
	@echo "Archivos fuente:"
	@dir *.cpp *.hpp 2>nul | findstr /V "Directory"

help:
	@echo "Comandos: make, make todo, make run, make clean, make info, make help"

.PHONY: all todo run clean info help