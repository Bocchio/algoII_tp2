#CXX = g++
#CPPFLAGS = -std=c++98 -Wall -pedantic-errors -g -O0
CXX = clang++
CPPFLAGS = -std=c++98 -Weverything -pedantic-errors -g -O0

all: tp1 diff_complex performance

diff_complex: diff_complex.o cmdline.o
	$(CXX) $(CPPFLAGS) $^ -o $@

diff_complex.o: diff_complex.cpp vector.hpp complex.hpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

tp1: main.o cmdline.o
	$(CXX) $(CPPFLAGS) $^ -o $@

cmdline.o: cmdline.cpp
	$(CXX) -c $(CPPFLAGS) $^ -o $@

main.o: main.cpp complex.hpp vector.hpp ft.hpp dictionary.hpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

performance: performance.o cmdline.o
	$(CXX) $(CPPFLAGS) $^ -o $@

performance.o: performance.cpp complex.hpp vector.hpp ft.hpp dictionary.hpp
	$(CXX) -c $(CPPFLAGS) $< -o $@

clean :
	rm -f *.o
