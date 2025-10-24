# Makefile para Obligatorio EDA 2025 - Versión Windows
# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# Lista de todos los archivos objeto
OBJETOS = main.o archivo.o version.o texto.o arbol_versiones.o

# Archivo ejecutable principal
TARGET = main.exe

# Regla principal - compilar todo
$(TARGET): $(OBJETOS)
	$(CXX) -o $(TARGET) $(OBJETOS)

# Dependencias de cada archivo objeto
main.o: main.cpp archivo.hpp version.hpp texto.hpp linea.hpp definiciones.hpp arbol_versiones.hpp estructuras_comunes.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

archivo.o: archivo.cpp archivo.hpp version.hpp texto.hpp linea.hpp definiciones.hpp arbol_versiones.hpp estructuras_comunes.hpp
	$(CXX) $(CXXFLAGS) -c archivo.cpp

version.o: version.cpp version.hpp texto.hpp linea.hpp definiciones.hpp estructuras_comunes.hpp
	$(CXX) $(CXXFLAGS) -c version.cpp

texto.o: texto.cpp texto.hpp linea.hpp definiciones.hpp
	$(CXX) $(CXXFLAGS) -c texto.cpp

arbol_versiones.o: arbol_versiones.cpp arbol_versiones.hpp version.hpp texto.hpp linea.hpp definiciones.hpp estructuras_comunes.hpp
	$(CXX) $(CXXFLAGS) -c arbol_versiones.cpp

# Reglas para limpieza (Windows)
clean:
	del /Q *.o $(TARGET) 2>nul || true

# Regla para compilar todo
todo: $(TARGET)

# Reglas phony
.PHONY: clean clean-all todo