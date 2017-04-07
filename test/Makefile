TARGETS = test_pole
LIBS = ../*.o
LIBPATH = -I../include -L../

root_flags :=`root-config --cflags --libs` -lgsl -lgslcblas -fopenmp -lMinuit

all: $(TARGETS) $(LIBS)

test: $(TARGETS) $(addprefix exec-test-,$(TARGETS))

%: %.cpp
	$(CXX) -o $@.test $^ -lboost_unit_test_framework $(root_flags) $(LIBPATH) $(LIBS)

exec-test-%: %
	-./$^.test --log_level=test_suite

clean:
	rm *.test