# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# Informática Básica
#
# @author Fabián González Lence
# @date 11/03/2024
# @brief Makefile Hash

CXX = g++						         		 # The C++ compiler command
CXXFLAGS = -std=c++17 -g -Wall	 # The C++ compiler options (C++14, and warn all)
LDFLAGS =						         		 # The linker options (if any)

# The all target builds all of the programs handled by the makefile.
all: Hash 

# The Hash target builds the Hash executable.
Hash: src/tools.o src/main.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	rm src/*.o
	
# Indicate that the all and clean targets do not
# correspond to actual files.
.PHONY: all clean
	
# The following rule is effectively built into make and
# therefore need not be explicitly specified:
# Hash.o: Hash.cc
#
	$(CXX) $(CXXFLAGS) -c $<

# The clean target removes all of the executable files
# and object files produced by the build process
# We can use it for additional housekeeping purposes
clean :
	rm -f Hash src/*.o
	rm -rf *~ basura* b i
	rm -rf a.out
	find . -name '*~' -exec rm {} \;
	find . -name basura -exec rm {} \;

push:
	git pull 
	git add . 
	git commit -m "Minor changes" 
	git push