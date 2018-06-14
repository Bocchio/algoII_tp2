#CXX = g++
#CPPFLAGS = -std=c++98 -Wall -pedantic-errors -g -O0
CXX = clang++
WARNINGS = -Wall -Wno-global-constructors #-Wno-weak-vtables -Wno-padded -Wno-reserved-id-macro -Wno-shadow -Wglobal-constructors
CPPFLAGS = -std=c++98 -pedantic-errors -g -O0

all: tp2 #diff_complex performance

#diff_complex: diff_complex.o cmdline.o
#	$(CXX) $(CPPFLAGS) $^ -o $@

#diff_complex.o: diff_complex.cpp vector.hpp complex.hpp
#	$(CXX) -c $(CPPFLAGS) $< -o $@

tp2: main.o cmdline.o
	$(CXX) $(CPPFLAGS) $(WARNINGS) $^ -o $@

cmdline.o: cmdline.cpp
	$(CXX) -c $(CPPFLAGS) $^ -o $@

main.o: main.cpp complex.hpp vector.hpp ft.hpp dictionary.hpp
	$(CXX) -c $(CPPFLAGS) $(WARNINGS) $< -o $@

#performance: performance.o cmdline.o
#	$(CXX) $(CPPFLAGS) $^ -o $@

#performance.o: performance.cpp complex.hpp vector.hpp ft.hpp dictionary.hpp
#	$(CXX) -c $(CPPFLAGS) $< -o $@

clean :
	rm -f *.o
