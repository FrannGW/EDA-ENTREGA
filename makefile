# Makefile para Obligatorio EDA 2025 - Versión Windows
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -g

# Lista de todos los archivos objeto
OBJETOS = main.o archivo.o version.o NavegacionVersiones.o ManipulacionVersiones.o CreacionVersiones.o EliminacionVersiones.o MostrarVersiones.o

TARGET = main.exe

$(TARGET): $(OBJETOS)
	$(CXX) -o $(TARGET) $(OBJETOS)

main.o: main.cpp archivo.hpp version.hpp definiciones.hpp estructuras_comunes.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

archivo.o: archivo.cpp archivo.hpp version.hpp definiciones.hpp estructuras_comunes.hpp CreacionVersiones.hpp EliminacionVersiones.hpp MostrarVersiones.hpp NavegacionVersiones.hpp
	$(CXX) $(CXXFLAGS) -c archivo.cpp

version.o: version.cpp version.hpp definiciones.hpp estructuras_comunes.hpp
	$(CXX) $(CXXFLAGS) -c version.cpp

# NUEVOS MÓDULOS ESPECIALIZADOS
NavegacionVersiones.o: NavegacionVersiones.cpp NavegacionVersiones.hpp
	$(CXX) $(CXXFLAGS) -c NavegacionVersiones.cpp

ManipulacionVersiones.o: ManipulacionVersiones.cpp ManipulacionVersiones.hpp NavegacionVersiones.hpp
	$(CXX) $(CXXFLAGS) -c ManipulacionVersiones.cpp

# MÓDULOS DE OPERACIONES (actualizados)
CreacionVersiones.o: CreacionVersiones.cpp CreacionVersiones.hpp NavegacionVersiones.hpp ManipulacionVersiones.hpp version.hpp archivo.hpp
	$(CXX) $(CXXFLAGS) -c CreacionVersiones.cpp

EliminacionVersiones.o: EliminacionVersiones.cpp EliminacionVersiones.hpp NavegacionVersiones.hpp archivo.hpp
	$(CXX) $(CXXFLAGS) -c EliminacionVersiones.cpp

MostrarVersiones.o: MostrarVersiones.cpp MostrarVersiones.hpp archivo.hpp
	$(CXX) $(CXXFLAGS) -c MostrarVersiones.cpp

clean:
	del /Q *.o $(TARGET) 2>nul || true

todo: $(TARGET)

.PHONY: clean clean-all todo