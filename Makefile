CPP       = mpic++


PYTHON_VERSION := $(shell python3 -c "import sys; print(f'{sys.version_info.major}.{sys.version_info.minor}')")
PYTHON_INCLUDE := $(shell python3-config --includes)
PYBIND11_INCLUDES := $(shell python3 -m pybind11 --includes)
PYTHON_LDFLAGS := $(shell python3-config --ldflags)
PYTHON_LIBS := $(shell python3-config --libs)

# Add explicit python lib
PYTHON_LIB := -lpython$(PYTHON_VERSION) $(PYTHON_LDFLAGS) $(PYTHON_LIBS)

INCLUDES += $(PYBIND11_INCLUDES) $(PYTHON_INCLUDE)
LIB += $(PYTHON_LIB)

LOG_LEVEL ?= 0

# source file(s)
SRCS = src/test_module.cpp

# default target
all: clean test_module.so

test_module.so: $(SRCS)
	$(CPP) -shared -fPIC -fno-common -DLOG_LEVEL=$(LOG_LEVEL) $(INCLUDES) $^ -o $@ $(LIB)

clean:
	rm -f test_module.so
