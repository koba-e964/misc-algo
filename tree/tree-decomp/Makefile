CXXFLAGS = -Wall -Wextra -Wpedantic
OBJS = td-main max-indep-main decomp-main decomp-max-indep

.PHONY : clean

td-main : td.cpp td.h td-main.cpp
	$(CXX) $(CXXFLAGS) td.cpp td-main.cpp -o $@
max-indep-main : td.cpp td.h max-indep.cpp max-indep.h max-indep-main.cpp
	$(CXX) $(CXXFLAGS) td.cpp max-indep.cpp max-indep-main.cpp -o $@
decomp-main: td.cpp td.h decomp.cpp decomp.h decomp-main.cpp
	$(CXX) $(CXXFLAGS) td.cpp decomp.cpp decomp-main.cpp -o $@
decomp-max-indep: td.cpp td.h decomp.cpp decomp.h max-indep.cpp max-indep.h decomp-max-indep.cpp
	$(CXX) $(CXXFLAGS) td.cpp decomp.cpp max-indep.cpp decomp-max-indep.cpp -o $@
clean :
	$(RM) $(OBJS)
