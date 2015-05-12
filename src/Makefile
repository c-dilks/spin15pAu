ARCH = linux

ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs)
ROOTGLIBS := $(shell root-config --glibs)

ifeq ($(ARCH),linux)
CXX = g++
CXXFLAGS = -O -Wno-deprecated -fPIC -m32 -fno-inline -Wno-write-strings
LD = g++
LDFLAGS = -O
SOFLAGS = -shared
INCLUDE = $(ROOTSYS)/include/
endif

CXXFLAGS += $(ROOTCFLAGS)
LIBS = $(ROOTLIBS) $(SYSLIBS)
GLIBS = $(ROOTGLIBS) $(SYSLIBS)

#-----------------------------------------------

CXXFILES = Environ.cxx RunData.cxx Trigger.cxx EventClass.cxx TCUbits.cxx
HEADERS = Environ.h RunData.h Trigger.h EventClass.h TCUbits.h

#-----------------------------------------------

All: RunData.so

RunData.so: LinkDef.h $(HEADERS) $(CXXFILES)
	@echo "generating dictionary $@..."
	@rootcint -f RunDataDict.cxx -c $(HEADERS) LinkDef.h
	g++ $(CXXFILES) RunDataDict.cxx -shared -o RunData.so $(CXXFLAGS) $(GLIBS)

clean:
	@rm RunData.so
