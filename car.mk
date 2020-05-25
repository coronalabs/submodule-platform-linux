##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=car
ConfigurationName      :=Debug
WorkspacePath          :=../../platform/linux
ProjectPath            :=../../platform/linux
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Robert
Date                   :=12/04/20
CodeLitePath           :=/home/robert/.codelite
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)Rtt_LINUX_ENV $(PreprocessorSwitch)Rtt_NO_ARCHIVE 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="car.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../librtt $(IncludeSwitch)../../librtt/Core 
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
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_up_tools_car_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_librtt_Core_Rtt_String.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_tools_car_Rtt_Car.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_librtt_Rtt_Archive.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_tools_car_main.cpp$(ObjectSuffix): ../../tools/car/main.cpp $(IntermediateDirectory)/up_up_tools_car_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../tools/car/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_tools_car_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_tools_car_main.cpp$(DependSuffix): ../../tools/car/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_tools_car_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_tools_car_main.cpp$(DependSuffix) -MM ../../tools/car/main.cpp

$(IntermediateDirectory)/up_up_tools_car_main.cpp$(PreprocessSuffix): ../../tools/car/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_tools_car_main.cpp$(PreprocessSuffix) ../../tools/car/main.cpp

$(IntermediateDirectory)/up_up_librtt_Core_Rtt_String.cpp$(ObjectSuffix): ../../librtt/Core/Rtt_String.cpp $(IntermediateDirectory)/up_up_librtt_Core_Rtt_String.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../librtt/Core/Rtt_String.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_librtt_Core_Rtt_String.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_librtt_Core_Rtt_String.cpp$(DependSuffix): ../../librtt/Core/Rtt_String.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_librtt_Core_Rtt_String.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_librtt_Core_Rtt_String.cpp$(DependSuffix) -MM ../../librtt/Core/Rtt_String.cpp

$(IntermediateDirectory)/up_up_librtt_Core_Rtt_String.cpp$(PreprocessSuffix): ../../librtt/Core/Rtt_String.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_librtt_Core_Rtt_String.cpp$(PreprocessSuffix) ../../librtt/Core/Rtt_String.cpp

$(IntermediateDirectory)/up_up_tools_car_Rtt_Car.cpp$(ObjectSuffix): ../../tools/car/Rtt_Car.cpp $(IntermediateDirectory)/up_up_tools_car_Rtt_Car.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../tools/car/Rtt_Car.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_tools_car_Rtt_Car.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_tools_car_Rtt_Car.cpp$(DependSuffix): ../../tools/car/Rtt_Car.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_tools_car_Rtt_Car.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_tools_car_Rtt_Car.cpp$(DependSuffix) -MM ../../tools/car/Rtt_Car.cpp

$(IntermediateDirectory)/up_up_tools_car_Rtt_Car.cpp$(PreprocessSuffix): ../../tools/car/Rtt_Car.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_tools_car_Rtt_Car.cpp$(PreprocessSuffix) ../../tools/car/Rtt_Car.cpp

$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(ObjectSuffix): ../../librtt/Core/Rtt_FileSystem.cpp $(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../librtt/Core/Rtt_FileSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(DependSuffix): ../../librtt/Core/Rtt_FileSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(DependSuffix) -MM ../../librtt/Core/Rtt_FileSystem.cpp

$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(PreprocessSuffix): ../../librtt/Core/Rtt_FileSystem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(PreprocessSuffix) ../../librtt/Core/Rtt_FileSystem.cpp

$(IntermediateDirectory)/up_up_librtt_Rtt_Archive.cpp$(ObjectSuffix): ../../librtt/Rtt_Archive.cpp $(IntermediateDirectory)/up_up_librtt_Rtt_Archive.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "../../librtt/Rtt_Archive.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_librtt_Rtt_Archive.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_librtt_Rtt_Archive.cpp$(DependSuffix): ../../librtt/Rtt_Archive.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_librtt_Rtt_Archive.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_librtt_Rtt_Archive.cpp$(DependSuffix) -MM ../../librtt/Rtt_Archive.cpp

$(IntermediateDirectory)/up_up_librtt_Rtt_Archive.cpp$(PreprocessSuffix): ../../librtt/Rtt_Archive.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_librtt_Rtt_Archive.cpp$(PreprocessSuffix) ../../librtt/Rtt_Archive.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


