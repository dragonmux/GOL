include Makefile.inc

EXTRA_CFLAGS = $(shell pkg-config --cflags libGTK++)
CFLAGS = -c -O2 -I../ $(EXTRA_CFLAGS) -o $@ $<
LIBS = $(shell pkg-config --libs libGTK++) -lstdc++
LFLAGS = $(O) $(LIBS) -o $(EXE)

O = GOL.o
EXE = GOL

default: all

all: $(EXE)

$(EXE): $(O)
	$(call run-cmd,ccld,$(LFLAGS))
	$(call run-cmd,chmod,$(EXE))
	$(call debug-strip,$(EXE))

clean:
	$(call run-cmd,rm,GOL,$(EXE) $(O))

.c.o:
	$(call run-cmd,cc,$(CFLAGS))

.cpp.o:
	$(call run-cmd,cxx,$(CFLAGS))

.PHONY: default all clean .c.o .cpp.o
