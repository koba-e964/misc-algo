CXXFLAGS = -Wall -Wextra -Wpedantic
OBJS = td-main max-indep decomp-main

.PHONY : clean

td-main : td.cpp td.h td-main.cpp
	$(CXX) $(CXXFLAGS) td.cpp td-main.cpp -o $@
max-indep : td.cpp td.h max-indep.cpp
	$(CXX) $(CXXFLAGS) td.cpp max-indep.cpp -o $@
decomp-main: td.cpp td.h decomp.cpp decomp.h decomp-main.cpp
	$(CXX) $(CXXFLAGS) td.cpp decomp.cpp decomp-main.cpp -o $@
clean :
	$(RM) $(OBJS)
