CXXFLAGS = -Wall -Wextra -Wpedantic
OBJS = td-main max-indep-main decomp-main decomp-max-indep max-indep-exp random-decomp decomp-slib-max-indep

.PHONY : clean

td-main : td.cpp td.h td-main.cpp
	$(CXX) $(CXXFLAGS) td.cpp td-main.cpp -o $@
max-indep-main : td.cpp td.h max-indep.cpp max-indep.h max-indep-main.cpp
	$(CXX) $(CXXFLAGS) td.cpp max-indep.cpp max-indep-main.cpp -o $@
decomp-main: td.cpp td.h decomp.cpp decomp.h decomp-main.cpp
	$(CXX) $(CXXFLAGS) td.cpp decomp.cpp decomp-main.cpp -o $@
decomp-max-indep: td.cpp td.h decomp.cpp decomp.h max-indep.cpp max-indep.h decomp-max-indep.cpp
	$(CXX) $(CXXFLAGS) td.cpp decomp.cpp max-indep.cpp decomp-max-indep.cpp -o $@
decomp-slib-max-indep: td.cpp td.h decomp.cpp decomp.h max-indep.cpp max-indep.h decomp-slib-max-indep.cpp
	$(CXX) $(CXXFLAGS) td.cpp decomp.cpp max-indep.cpp decomp-slib-max-indep.cpp -o $@
max-indep-exp : max-indep-exp.cpp
	$(CXX) $(CXXFLAGS) max-indep-exp.cpp -o $@
random-decomp: td.cpp td.h decomp.cpp decomp.h random-decomp.cpp
	$(CXX) $(CXXFLAGS) td.cpp decomp.cpp random-decomp.cpp -o $@
indep/%.test : max-indep-exp decomp-max-indep indep/%.txt
	./max-indep-exp <indep/$*.txt
	./decomp-max-indep <indep/$*.txt
clean :
	$(RM) $(OBJS)
