TARGETS = test_pole test_amplitude draw_results test_amplitude_mpi test_minuit_mpi
SOURCES = ../src/*.cc ../include/*.h
LIBS = ../*.o
LIBPATH = -I../include -L../
CXX = mpic++

root_flags :=`root-config --cflags --libs` -lgsl -lgslcblas -lMinuit -fopenmp

all: $(test)

TARGETS: $(TARGETS) $(addprefix exec-test-,$(TARGETS))


$(LIBS): $(SOURCES)
	cd ../; make

%: %.cpp $(LIBS)
	$(CXX) -o $@.test $^ -lboost_unit_test_framework $(root_flags) $(LIBPATH)

exec-test-%: %
	-./$^.test --log_level=test_suite

clean:
	rm *.test

	
check-performance:
	./run-performance.sh