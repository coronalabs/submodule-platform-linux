##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=lua
ConfigurationName      :=Release
WorkspacePath          :=../../platform/linux
ProjectPath            :=../../platform/linux
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=vitaly
Date                   :=19/09/18
CodeLitePath           :=/home/vitaly/.codelite
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="lua.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -fPIC $(Preprocessors)
CFLAGS   :=  -fPIC $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lzio.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luac.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltm.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltable.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lvm.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstrlib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lfunc.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmathlib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lgc.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltablib.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lcode.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luaconf.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lbaselib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lapi.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lundump.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstring.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldump.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lauxlib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldebug.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_linit.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_liolib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_llex.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmem.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loadlib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lua.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstate.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lobject.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lopcodes.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldblib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldo.c$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loslib.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lparser.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "../../platform/linux/.build-release"
	@echo rebuilt > "../../platform/linux/.build-release/lua"

MakeIntermediateDirs:
	@test -d ./Release || $(MakeDirCommand) ./Release


./Release:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lzio.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lzio.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lzio.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lzio.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lzio.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lzio.c$(DependSuffix): ../../external/lua-5.1.3/src/lzio.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lzio.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lzio.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lzio.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lzio.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lzio.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lzio.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lzio.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luac.c$(ObjectSuffix): ../../external/lua-5.1.3/src/luac.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luac.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/luac.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luac.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luac.c$(DependSuffix): ../../external/lua-5.1.3/src/luac.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luac.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luac.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/luac.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luac.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/luac.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luac.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/luac.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltm.c$(ObjectSuffix): ../../external/lua-5.1.3/src/ltm.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltm.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/ltm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltm.c$(DependSuffix): ../../external/lua-5.1.3/src/ltm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltm.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltm.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/ltm.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltm.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/ltm.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltm.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/ltm.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltable.c$(ObjectSuffix): ../../external/lua-5.1.3/src/ltable.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltable.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/ltable.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltable.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltable.c$(DependSuffix): ../../external/lua-5.1.3/src/ltable.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltable.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltable.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/ltable.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltable.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/ltable.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltable.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/ltable.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lvm.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lvm.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lvm.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lvm.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lvm.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lvm.c$(DependSuffix): ../../external/lua-5.1.3/src/lvm.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lvm.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lvm.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lvm.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lvm.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lvm.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lvm.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lvm.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstrlib.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lstrlib.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstrlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lstrlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstrlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstrlib.c$(DependSuffix): ../../external/lua-5.1.3/src/lstrlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstrlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstrlib.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lstrlib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstrlib.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lstrlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstrlib.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lstrlib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lfunc.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lfunc.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lfunc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lfunc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lfunc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lfunc.c$(DependSuffix): ../../external/lua-5.1.3/src/lfunc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lfunc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lfunc.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lfunc.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lfunc.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lfunc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lfunc.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lfunc.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmathlib.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lmathlib.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmathlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lmathlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmathlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmathlib.c$(DependSuffix): ../../external/lua-5.1.3/src/lmathlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmathlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmathlib.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lmathlib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmathlib.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lmathlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmathlib.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lmathlib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lgc.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lgc.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lgc.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lgc.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lgc.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lgc.c$(DependSuffix): ../../external/lua-5.1.3/src/lgc.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lgc.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lgc.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lgc.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lgc.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lgc.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lgc.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lgc.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltablib.c$(ObjectSuffix): ../../external/lua-5.1.3/src/ltablib.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltablib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/ltablib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltablib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltablib.c$(DependSuffix): ../../external/lua-5.1.3/src/ltablib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltablib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltablib.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/ltablib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltablib.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/ltablib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ltablib.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/ltablib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lcode.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lcode.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lcode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lcode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lcode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lcode.c$(DependSuffix): ../../external/lua-5.1.3/src/lcode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lcode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lcode.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lcode.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lcode.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lcode.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lcode.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lcode.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luaconf.c$(ObjectSuffix): ../../external/lua-5.1.3/src/luaconf.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luaconf.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/luaconf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luaconf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luaconf.c$(DependSuffix): ../../external/lua-5.1.3/src/luaconf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luaconf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luaconf.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/luaconf.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luaconf.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/luaconf.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_luaconf.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/luaconf.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lbaselib.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lbaselib.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lbaselib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lbaselib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lbaselib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lbaselib.c$(DependSuffix): ../../external/lua-5.1.3/src/lbaselib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lbaselib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lbaselib.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lbaselib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lbaselib.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lbaselib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lbaselib.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lbaselib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lapi.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lapi.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lapi.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lapi.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lapi.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lapi.c$(DependSuffix): ../../external/lua-5.1.3/src/lapi.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lapi.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lapi.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lapi.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lapi.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lapi.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lapi.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lapi.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lundump.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lundump.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lundump.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lundump.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lundump.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lundump.c$(DependSuffix): ../../external/lua-5.1.3/src/lundump.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lundump.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lundump.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lundump.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lundump.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lundump.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lundump.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lundump.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstring.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lstring.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstring.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lstring.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstring.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstring.c$(DependSuffix): ../../external/lua-5.1.3/src/lstring.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstring.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstring.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lstring.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstring.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lstring.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstring.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lstring.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldump.c$(ObjectSuffix): ../../external/lua-5.1.3/src/ldump.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldump.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/ldump.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldump.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldump.c$(DependSuffix): ../../external/lua-5.1.3/src/ldump.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldump.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldump.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/ldump.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldump.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/ldump.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldump.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/ldump.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lauxlib.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lauxlib.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lauxlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lauxlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lauxlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lauxlib.c$(DependSuffix): ../../external/lua-5.1.3/src/lauxlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lauxlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lauxlib.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lauxlib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lauxlib.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lauxlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lauxlib.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lauxlib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldebug.c$(ObjectSuffix): ../../external/lua-5.1.3/src/ldebug.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldebug.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/ldebug.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldebug.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldebug.c$(DependSuffix): ../../external/lua-5.1.3/src/ldebug.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldebug.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldebug.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/ldebug.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldebug.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/ldebug.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldebug.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/ldebug.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_linit.c$(ObjectSuffix): ../../external/lua-5.1.3/src/linit.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_linit.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/linit.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_linit.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_linit.c$(DependSuffix): ../../external/lua-5.1.3/src/linit.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_linit.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_linit.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/linit.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_linit.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/linit.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_linit.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/linit.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_liolib.c$(ObjectSuffix): ../../external/lua-5.1.3/src/liolib.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_liolib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/liolib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_liolib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_liolib.c$(DependSuffix): ../../external/lua-5.1.3/src/liolib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_liolib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_liolib.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/liolib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_liolib.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/liolib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_liolib.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/liolib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_llex.c$(ObjectSuffix): ../../external/lua-5.1.3/src/llex.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_llex.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/llex.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_llex.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_llex.c$(DependSuffix): ../../external/lua-5.1.3/src/llex.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_llex.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_llex.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/llex.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_llex.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/llex.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_llex.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/llex.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmem.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lmem.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmem.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lmem.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmem.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmem.c$(DependSuffix): ../../external/lua-5.1.3/src/lmem.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmem.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmem.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lmem.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmem.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lmem.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lmem.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lmem.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loadlib.c$(ObjectSuffix): ../../external/lua-5.1.3/src/loadlib.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loadlib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/loadlib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loadlib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loadlib.c$(DependSuffix): ../../external/lua-5.1.3/src/loadlib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loadlib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loadlib.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/loadlib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loadlib.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/loadlib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loadlib.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/loadlib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lua.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lua.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lua.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lua.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lua.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lua.c$(DependSuffix): ../../external/lua-5.1.3/src/lua.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lua.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lua.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lua.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lua.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lua.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lua.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lua.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstate.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lstate.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstate.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lstate.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstate.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstate.c$(DependSuffix): ../../external/lua-5.1.3/src/lstate.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstate.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstate.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lstate.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstate.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lstate.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lstate.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lstate.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lobject.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lobject.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lobject.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lobject.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lobject.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lobject.c$(DependSuffix): ../../external/lua-5.1.3/src/lobject.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lobject.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lobject.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lobject.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lobject.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lobject.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lobject.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lobject.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lopcodes.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lopcodes.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lopcodes.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lopcodes.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lopcodes.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lopcodes.c$(DependSuffix): ../../external/lua-5.1.3/src/lopcodes.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lopcodes.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lopcodes.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lopcodes.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lopcodes.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lopcodes.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lopcodes.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lopcodes.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldblib.c$(ObjectSuffix): ../../external/lua-5.1.3/src/ldblib.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldblib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/ldblib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldblib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldblib.c$(DependSuffix): ../../external/lua-5.1.3/src/ldblib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldblib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldblib.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/ldblib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldblib.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/ldblib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldblib.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/ldblib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldo.c$(ObjectSuffix): ../../external/lua-5.1.3/src/ldo.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldo.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/ldo.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldo.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldo.c$(DependSuffix): ../../external/lua-5.1.3/src/ldo.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldo.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldo.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/ldo.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldo.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/ldo.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_ldo.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/ldo.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loslib.c$(ObjectSuffix): ../../external/lua-5.1.3/src/loslib.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loslib.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/loslib.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loslib.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loslib.c$(DependSuffix): ../../external/lua-5.1.3/src/loslib.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loslib.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loslib.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/loslib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loslib.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/loslib.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_loslib.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/loslib.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lparser.c$(ObjectSuffix): ../../external/lua-5.1.3/src/lparser.c $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lparser.c$(DependSuffix)
	$(CC) $(SourceSwitch) "../../external/lua-5.1.3/src/lparser.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lparser.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lparser.c$(DependSuffix): ../../external/lua-5.1.3/src/lparser.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lparser.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lparser.c$(DependSuffix) -MM ../../external/lua-5.1.3/src/lparser.c

$(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lparser.c$(PreprocessSuffix): ../../external/lua-5.1.3/src/lparser.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_lua-5.1.3_src_lparser.c$(PreprocessSuffix) ../../external/lua-5.1.3/src/lparser.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


