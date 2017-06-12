TARGETS = test_pole test_amplitude draw_results test_amplitude_mpi
LIBS = ../*.o
LIBPATH = -I../include -L../
CXX = mpicxx

root_flags :=`root-config --cflags --libs` -lgsl -lgslcblas -lMinuit -fopenmp

all: $(TARGETS) $(LIBS)

test: $(TARGETS) $(addprefix exec-test-,$(TARGETS))

%: %.cpp
	cd ../; make
	$(CXX) -o $@.test $^ -lboost_unit_test_framework $(root_flags) $(LIBPATH) $(LIBS)

exec-test-%: %
	-./$^.test --log_level=test_suite

clean:
	rm *.test