# This file is part of GOL
# Copyright © 2013 Rachel Mant (dx-mon@users.sourceforge.net)
#
# GOL is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GOL is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

include Makefile.inc

EXTRA_CFLAGS = $(shell pkg-config --cflags libGTK++)
CFLAGS = -c -O2 -I../ $(EXTRA_CFLAGS) -o $@ $<
LIBS = $(shell pkg-config --libs libGTK++)
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
