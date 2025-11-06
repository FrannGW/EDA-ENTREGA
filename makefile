# Makefile para Obligatorio EDA 2025 - Versión Windows
# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# Lista de todos los archivos objeto
OBJETOS = main.o archivo.o version.o arbol_versiones.o CreacionVersiones.o EliminacionVersiones.o MostrarVersiones.o

# Archivo ejecutable principal
TARGET = main.exe

# Regla principal - compilar todo
$(TARGET): $(OBJETOS)
	$(CXX) -o $(TARGET) $(OBJETOS)

# Dependencias de cada archivo objeto
main.o: main.cpp archivo.hpp version.hpp definiciones.hpp arbol_versiones.hpp estructuras_comunes.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

archivo.o: archivo.cpp archivo.hpp version.hpp definiciones.hpp arbol_versiones.hpp estructuras_comunes.hpp CreacionVersiones.hpp EliminacionVersiones.hpp MostrarVersiones.hpp
	$(CXX) $(CXXFLAGS) -c archivo.cpp

version.o: version.cpp version.hpp definiciones.hpp estructuras_comunes.hpp
	$(CXX) $(CXXFLAGS) -c version.cpp

arbol_versiones.o: arbol_versiones.cpp arbol_versiones.hpp version.hpp definiciones.hpp estructuras_comunes.hpp
	$(CXX) $(CXXFLAGS) -c arbol_versiones.cpp

# NUEVOS MÓDULOS
CreacionVersiones.o: CreacionVersiones.cpp CreacionVersiones.hpp arbol_versiones.hpp version.hpp
	$(CXX) $(CXXFLAGS) -c CreacionVersiones.cpp

EliminacionVersiones.o: EliminacionVersiones.cpp EliminacionVersiones.hpp arbol_versiones.hpp
	$(CXX) $(CXXFLAGS) -c EliminacionVersiones.cpp

MostrarVersiones.o: MostrarVersiones.cpp MostrarVersiones.hpp arbol_versiones.hpp
	$(CXX) $(CXXFLAGS) -c MostrarVersiones.cpp

# Reglas para limpieza (Windows)
clean:
	del /Q *.o $(TARGET) 2>nul || true

# Regla para compilar todo
todo: $(TARGET)

# Reglas phony
.PHONY: clean clean-all todo