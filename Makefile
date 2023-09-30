HOST_TARGET = Linux-x86_64-GCC
TESTFLOAT_OPTS ?= -DFLOAT64 -DFLOAT_ROUND_ODD
CXX = g++
INC_DIR = 	./berkeley-testfloat-3/source/ \
			./berkeley-softfloat-3/source/include/

all: generator

generator: src/main.c berkeley-softfloat-3/build/$(HOST_TARGET)/softfloat.a berkeley-testfloat-3/build/$(HOST_TARGET)/testfloat.a
	$(CXX) $^ $(addprefix -I,${INC_DIR}) -g -o $@

init:
	git submodule update --init --recursive

clean:
	$(MAKE) -C berkeley-softfloat-3/build/$(HOST_TARGET) clean
	$(MAKE) -C berkeley-testfloat-3/build/$(HOST_TARGET) clean
	rm generator

berkeley-softfloat-3/build/$(HOST_TARGET)/softfloat.a:
	$(MAKE) -C berkeley-softfloat-3/build/$(HOST_TARGET) TESTFLOAT_OPTS="$(TESTFLOAT_OPTS)" softfloat.a -j `nproc`

berkeley-testfloat-3/build/$(HOST_TARGET)/testfloat.a:
	$(MAKE) -C berkeley-testfloat-3/build/$(HOST_TARGET) TESTFLOAT_OPTS="$(TESTFLOAT_OPTS)" testfloat.a -j `nproc`