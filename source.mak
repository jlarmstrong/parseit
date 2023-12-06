# Microsoft Developer Studio Generated NMAKE File, Based on source.dsp
!IF "$(CFG)" == ""
CFG=source - Win32 Debug
!MESSAGE No configuration specified. Defaulting to source - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "source - Win32 Release" && "$(CFG)" != "source - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "source.mak" CFG="source - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "source - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "source - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "source - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ParseIt.exe"


CLEAN :
	-@erase "$(INTDIR)\configFile.obj"
	-@erase "$(INTDIR)\htmlFormatter.obj"
	-@erase "$(INTDIR)\myLog.obj"
	-@erase "$(INTDIR)\playerList.obj"
	-@erase "$(INTDIR)\serverLog.obj"
	-@erase "$(INTDIR)\serverStat.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\ParseIt.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\source.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\source.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ParseIt.pdb" /machine:I386 /out:"$(OUTDIR)\ParseIt.exe" 
LINK32_OBJS= \
	"$(INTDIR)\configFile.obj" \
	"$(INTDIR)\htmlFormatter.obj" \
	"$(INTDIR)\myLog.obj" \
	"$(INTDIR)\playerList.obj" \
	"$(INTDIR)\serverLog.obj" \
	"$(INTDIR)\serverStat.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\ParseIt.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "source - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ParseIt.exe"


CLEAN :
	-@erase "$(INTDIR)\configFile.obj"
	-@erase "$(INTDIR)\htmlFormatter.obj"
	-@erase "$(INTDIR)\myLog.obj"
	-@erase "$(INTDIR)\playerList.obj"
	-@erase "$(INTDIR)\serverLog.obj"
	-@erase "$(INTDIR)\serverStat.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\ParseIt.exe"
	-@erase "$(OUTDIR)\ParseIt.ilk"
	-@erase "$(OUTDIR)\ParseIt.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "d:\parseit\source\base\\" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\source.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\source.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ParseIt.pdb" /debug /machine:I386 /include:"d:\parseit\source\base\killmsg.h" /out:"$(OUTDIR)\ParseIt.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\configFile.obj" \
	"$(INTDIR)\htmlFormatter.obj" \
	"$(INTDIR)\myLog.obj" \
	"$(INTDIR)\playerList.obj" \
	"$(INTDIR)\serverLog.obj" \
	"$(INTDIR)\serverStat.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\ParseIt.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("source.dep")
!INCLUDE "source.dep"
!ELSE 
!MESSAGE Warning: cannot find "source.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "source - Win32 Release" || "$(CFG)" == "source - Win32 Debug"
SOURCE=.\configFile.cpp

"$(INTDIR)\configFile.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\htmlFormatter.cpp

"$(INTDIR)\htmlFormatter.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\myLog.cpp

"$(INTDIR)\myLog.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\playerList.cpp

"$(INTDIR)\playerList.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\serverLog.cpp

"$(INTDIR)\serverLog.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\serverStat.cpp

"$(INTDIR)\serverStat.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\StdAfx.cpp

"$(INTDIR)\StdAfx.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

