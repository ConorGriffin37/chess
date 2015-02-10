
EXTRA_DIST =  

# Warning: This is an automatically generated file, do not edit!

if ENABLE_DEBUG_X86
ASSEMBLY_COMPILER_COMMAND = dmcs
ASSEMBLY_COMPILER_FLAGS =  -noconfig -codepage:utf8 -warn:4 -optimize- -debug "-define:DEBUG;"
ASSEMBLY = bin/Debug/test.dll
ASSEMBLY_MDB = $(ASSEMBLY).mdb
COMPILE_TARGET = library
PROJECT_REFERENCES =  \
	../GUI/bin/Debug/gui.exe
BUILD_DIR = bin/Debug

TEST_DLL_MDB_SOURCE=bin/Debug/test.dll.mdb
TEST_DLL_MDB=$(BUILD_DIR)/test.dll.mdb
GUI_EXE_SOURCE=../GUI/bin/Debug/gui.exe
GUI_EXE_MDB_SOURCE=../GUI/bin/Debug/gui.exe.mdb

endif

if ENABLE_RELEASE_X86
ASSEMBLY_COMPILER_COMMAND = dmcs
ASSEMBLY_COMPILER_FLAGS =  -noconfig -codepage:utf8 -warn:4 -optimize+
ASSEMBLY = bin/Release/test.dll
ASSEMBLY_MDB = 
COMPILE_TARGET = library
PROJECT_REFERENCES =  \
	../GUI/bin/Release/gui.exe
BUILD_DIR = bin/Release

TEST_DLL_MDB=
GUI_EXE_SOURCE=../GUI/bin/Debug/gui.exe
GUI_EXE_MDB_SOURCE=../GUI/bin/Debug/gui.exe.mdb

endif

AL=al
SATELLITE_ASSEMBLY_NAME=$(notdir $(basename $(ASSEMBLY))).resources.dll

PROGRAMFILES = \
	$(TEST_DLL_MDB) \
	$(GUI_EXE) \
	$(GUI_EXE_MDB)  

LINUX_PKGCONFIG = \
	$(TEST_PC)  


RESGEN=resgen2
	
all: $(ASSEMBLY) $(PROGRAMFILES) $(LINUX_PKGCONFIG) 

FILES =  \
	BoardTest.cs \
	EngineTest.cs \
	FENParserTest.cs \
	PieceValidMovesTest.cs 

DATA_FILES = 

RESOURCES = 

EXTRAS = \
	packages.config \
	test.pc.in 

REFERENCES =  \
	../GUI/bin/Debug/GUI.exe \
	-pkg:nunit \
	System

DLL_REFERENCES = 

CLEANFILES = $(PROGRAMFILES) $(LINUX_PKGCONFIG) 

include $(top_srcdir)/Makefile.include

GUI_EXE = $(BUILD_DIR)/gui.exe
GUI_EXE_MDB = $(BUILD_DIR)/gui.exe.mdb
TEST_PC = $(BUILD_DIR)/test.pc

$(eval $(call emit-deploy-target,GUI_EXE))
$(eval $(call emit-deploy-target,GUI_EXE_MDB))
$(eval $(call emit-deploy-wrapper,TEST_PC,test.pc))


$(eval $(call emit_resgen_targets))
$(build_xamlg_list): %.xaml.g.cs: %.xaml
	xamlg '$<'

$(ASSEMBLY_MDB): $(ASSEMBLY)

$(ASSEMBLY): $(build_sources) $(build_resources) $(build_datafiles) $(DLL_REFERENCES) $(PROJECT_REFERENCES) $(build_xamlg_list) $(build_satellite_assembly_list)
	mkdir -p $(shell dirname $(ASSEMBLY))
	$(ASSEMBLY_COMPILER_COMMAND) $(ASSEMBLY_COMPILER_FLAGS) -out:$(ASSEMBLY) -target:$(COMPILE_TARGET) $(build_sources_embed) $(build_resources_embed) $(build_references_ref)
