source_dirs := src
include_dirs := include
search_wildcard := $(addsuffix /*.cc,$(source_dirs))
root_flags :=`root-config --cflags --libs`
CC = mpicxx

profit: $(notdir $(patsubst   %.cc,%.o,$(wildcard   $(search_wildcard))))
	$(CC) main.cxx $^ -o $@ $(root_flags) -lgsl -lgslcblas -lMinuit -fopenmp $(addprefix -I ,$(include_dirs)) 
VPATH   := $(source_dirs)
%.o: %.cc
	$(CC) -c -MD $(addprefix -I ,$(include_dirs)) $< $(root_flags) -lMinuit -fopenmp
include $(wildcard   *.d)
.PHONY: clean
clean:
	rm *.o *.d

# run bath job on CIS cluster
#
launch:
	qsub -N t-nonlinear -q i14d -l nodes=1:ppn=40 cnonlinear-trajectory.sh 
