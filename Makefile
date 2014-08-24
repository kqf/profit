source_dirs := src
include_dirs := include
search_wildcard := $(addsuffix /*.cc,$(source_dirs))
root_flags :=`root-config --cflags --libs`

profit:main.o $(notdir $(patsubst   %.cc,%.o,$(wildcard   $(search_wildcard))))
	g++ $^ -o $@ $(root_flags) -lgsl -lgslcblas -lMinuit -fopenmp
VPATH   := $(source_dirs)
%.o: %.cc
	g++ -c -MD $(addprefix -I ,$(include_dirs)) $< $(root_flags) -lMinuit -fopenmp
include $(wildcard   *.d)
.PHONY: clean
clean:
	rm *.o *.d
