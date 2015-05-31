CXXFLAGS = -Wall -Wpedantic

td-main : td.cpp td.h td-main.cpp
	$(CXX) $(CXXFLAGS) td.cpp td-main.cpp -o $@
max-indep : td.cpp td.h max-indep.cpp
	$(CXX) $(CXXFLAGS) td.cpp max-indep.cpp -o $@
clean :
	$(RM) td-main max-indep
decomp: td.cpp td.h decomp.cpp
	$(CXX) $(CXXFLAGS) td.cpp decomp.cpp -o $@
