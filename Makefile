EXE = ParseIt

OBJ = configFile.o htmlFormatter.o myLog.o playerList.o \
      DynamicLibrary.o ParseModule.o serverLog.o serverStat.o StdAfx.o

CC =  g++

CDEFS = -DUNIX


.SUFFIXES: .cpp $(SUFFIXES)

.cpp.o:
	$(CC) -g -Wall $(CDEFS) -c $<


$(EXE):	$(OBJ)
	$(CC) -g -static -o $(EXE) $(OBJ) -ldl

clean:
	rm -f $(OBJ) core

realclean:
	rm -f $(EXE) $(OBJ) core
