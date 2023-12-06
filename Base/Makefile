EXE = base.so 

OBJ = killMsg.o basemodule.o StdAfx.o 

CC =  g++

CDEFS = -DUNIX


.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	$(CC) -fPIC -O2 -Wall $(CDEFS) -c $<


$(EXE):	$(OBJ)
	$(CC) -shared -Wl,-soname,base.so -o $(EXE) $(OBJ)

clean:
	rm -f $(OBJ) core

realclean:
	rm -f $(EXE) $(OBJ) core
