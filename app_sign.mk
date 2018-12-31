##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=app_sign
ConfigurationName      :=Release
WorkspacePath          :=/home/vitaly/main/platform/linux
ProjectPath            :=/home/vitaly/main/platform/linux
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=vitaly
Date                   :=27/11/18
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
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG $(PreprocessorSwitch)Rtt_LINUX_ENV $(PreprocessorSwitch)Rtt_NO_ARCHIVE 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="app_sign.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../librtt $(IncludeSwitch)../../librtt/Core $(IncludeSwitch)../shared $(IncludeSwitch)../../external/rsa 
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
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MD4.c$(ObjectSuffix) $(IntermediateDirectory)/up_up_librtt_Core_Rtt_MessageDigest.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_tools_app_sign_Rtt_AppSign.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_rsa_prime.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_tools_app_sign_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_rsa_vlong.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_shared_Rtt_AuthorizationTicket.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_rsa_vlong_io.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_external_rsa_rsa.cpp$(ObjectSuffix) $(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/up_up_librtt_Rtt_Message.cpp$(ObjectSuffix) 



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
	@test -d ./Release || $(MakeDirCommand) ./Release


$(IntermediateDirectory)/.d:
	@test -d ./Release || $(MakeDirCommand) ./Release

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MD4.c$(ObjectSuffix): ../../librtt/Core/Rtt_MD4.c $(IntermediateDirectory)/up_up_librtt_Core_Rtt_MD4.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/vitaly/main/librtt/Core/Rtt_MD4.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MD4.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MD4.c$(DependSuffix): ../../librtt/Core/Rtt_MD4.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MD4.c$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MD4.c$(DependSuffix) -MM ../../librtt/Core/Rtt_MD4.c

$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MD4.c$(PreprocessSuffix): ../../librtt/Core/Rtt_MD4.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_librtt_Core_Rtt_MD4.c$(PreprocessSuffix) ../../librtt/Core/Rtt_MD4.c

$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MessageDigest.cpp$(ObjectSuffix): ../../librtt/Core/Rtt_MessageDigest.cpp $(IntermediateDirectory)/up_up_librtt_Core_Rtt_MessageDigest.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/librtt/Core/Rtt_MessageDigest.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MessageDigest.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MessageDigest.cpp$(DependSuffix): ../../librtt/Core/Rtt_MessageDigest.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MessageDigest.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MessageDigest.cpp$(DependSuffix) -MM ../../librtt/Core/Rtt_MessageDigest.cpp

$(IntermediateDirectory)/up_up_librtt_Core_Rtt_MessageDigest.cpp$(PreprocessSuffix): ../../librtt/Core/Rtt_MessageDigest.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_librtt_Core_Rtt_MessageDigest.cpp$(PreprocessSuffix) ../../librtt/Core/Rtt_MessageDigest.cpp

$(IntermediateDirectory)/up_up_tools_app_sign_Rtt_AppSign.cpp$(ObjectSuffix): ../../tools/app_sign/Rtt_AppSign.cpp $(IntermediateDirectory)/up_up_tools_app_sign_Rtt_AppSign.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/tools/app_sign/Rtt_AppSign.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_tools_app_sign_Rtt_AppSign.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_tools_app_sign_Rtt_AppSign.cpp$(DependSuffix): ../../tools/app_sign/Rtt_AppSign.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_tools_app_sign_Rtt_AppSign.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_tools_app_sign_Rtt_AppSign.cpp$(DependSuffix) -MM ../../tools/app_sign/Rtt_AppSign.cpp

$(IntermediateDirectory)/up_up_tools_app_sign_Rtt_AppSign.cpp$(PreprocessSuffix): ../../tools/app_sign/Rtt_AppSign.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_tools_app_sign_Rtt_AppSign.cpp$(PreprocessSuffix) ../../tools/app_sign/Rtt_AppSign.cpp

$(IntermediateDirectory)/up_up_external_rsa_prime.cpp$(ObjectSuffix): ../../external/rsa/prime.cpp $(IntermediateDirectory)/up_up_external_rsa_prime.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/external/rsa/prime.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_rsa_prime.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_rsa_prime.cpp$(DependSuffix): ../../external/rsa/prime.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_rsa_prime.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_rsa_prime.cpp$(DependSuffix) -MM ../../external/rsa/prime.cpp

$(IntermediateDirectory)/up_up_external_rsa_prime.cpp$(PreprocessSuffix): ../../external/rsa/prime.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_rsa_prime.cpp$(PreprocessSuffix) ../../external/rsa/prime.cpp

$(IntermediateDirectory)/up_up_tools_app_sign_main.cpp$(ObjectSuffix): ../../tools/app_sign/main.cpp $(IntermediateDirectory)/up_up_tools_app_sign_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/tools/app_sign/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_tools_app_sign_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_tools_app_sign_main.cpp$(DependSuffix): ../../tools/app_sign/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_tools_app_sign_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_tools_app_sign_main.cpp$(DependSuffix) -MM ../../tools/app_sign/main.cpp

$(IntermediateDirectory)/up_up_tools_app_sign_main.cpp$(PreprocessSuffix): ../../tools/app_sign/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_tools_app_sign_main.cpp$(PreprocessSuffix) ../../tools/app_sign/main.cpp

$(IntermediateDirectory)/up_up_external_rsa_vlong.cpp$(ObjectSuffix): ../../external/rsa/vlong.cpp $(IntermediateDirectory)/up_up_external_rsa_vlong.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/external/rsa/vlong.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_rsa_vlong.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_rsa_vlong.cpp$(DependSuffix): ../../external/rsa/vlong.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_rsa_vlong.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_rsa_vlong.cpp$(DependSuffix) -MM ../../external/rsa/vlong.cpp

$(IntermediateDirectory)/up_up_external_rsa_vlong.cpp$(PreprocessSuffix): ../../external/rsa/vlong.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_rsa_vlong.cpp$(PreprocessSuffix) ../../external/rsa/vlong.cpp

$(IntermediateDirectory)/up_shared_Rtt_AuthorizationTicket.cpp$(ObjectSuffix): ../shared/Rtt_AuthorizationTicket.cpp $(IntermediateDirectory)/up_shared_Rtt_AuthorizationTicket.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/platform/shared/Rtt_AuthorizationTicket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_shared_Rtt_AuthorizationTicket.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_shared_Rtt_AuthorizationTicket.cpp$(DependSuffix): ../shared/Rtt_AuthorizationTicket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_shared_Rtt_AuthorizationTicket.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_shared_Rtt_AuthorizationTicket.cpp$(DependSuffix) -MM ../shared/Rtt_AuthorizationTicket.cpp

$(IntermediateDirectory)/up_shared_Rtt_AuthorizationTicket.cpp$(PreprocessSuffix): ../shared/Rtt_AuthorizationTicket.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_shared_Rtt_AuthorizationTicket.cpp$(PreprocessSuffix) ../shared/Rtt_AuthorizationTicket.cpp

$(IntermediateDirectory)/up_up_external_rsa_vlong_io.cpp$(ObjectSuffix): ../../external/rsa/vlong_io.cpp $(IntermediateDirectory)/up_up_external_rsa_vlong_io.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/external/rsa/vlong_io.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_rsa_vlong_io.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_rsa_vlong_io.cpp$(DependSuffix): ../../external/rsa/vlong_io.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_rsa_vlong_io.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_rsa_vlong_io.cpp$(DependSuffix) -MM ../../external/rsa/vlong_io.cpp

$(IntermediateDirectory)/up_up_external_rsa_vlong_io.cpp$(PreprocessSuffix): ../../external/rsa/vlong_io.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_rsa_vlong_io.cpp$(PreprocessSuffix) ../../external/rsa/vlong_io.cpp

$(IntermediateDirectory)/up_up_external_rsa_rsa.cpp$(ObjectSuffix): ../../external/rsa/rsa.cpp $(IntermediateDirectory)/up_up_external_rsa_rsa.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/external/rsa/rsa.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_external_rsa_rsa.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_external_rsa_rsa.cpp$(DependSuffix): ../../external/rsa/rsa.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_external_rsa_rsa.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_external_rsa_rsa.cpp$(DependSuffix) -MM ../../external/rsa/rsa.cpp

$(IntermediateDirectory)/up_up_external_rsa_rsa.cpp$(PreprocessSuffix): ../../external/rsa/rsa.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_external_rsa_rsa.cpp$(PreprocessSuffix) ../../external/rsa/rsa.cpp

$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(ObjectSuffix): ../../librtt/Core/Rtt_FileSystem.cpp $(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/librtt/Core/Rtt_FileSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(DependSuffix): ../../librtt/Core/Rtt_FileSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(DependSuffix) -MM ../../librtt/Core/Rtt_FileSystem.cpp

$(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(PreprocessSuffix): ../../librtt/Core/Rtt_FileSystem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_librtt_Core_Rtt_FileSystem.cpp$(PreprocessSuffix) ../../librtt/Core/Rtt_FileSystem.cpp

$(IntermediateDirectory)/up_up_librtt_Rtt_Message.cpp$(ObjectSuffix): ../../librtt/Rtt_Message.cpp $(IntermediateDirectory)/up_up_librtt_Rtt_Message.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/vitaly/main/librtt/Rtt_Message.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/up_up_librtt_Rtt_Message.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/up_up_librtt_Rtt_Message.cpp$(DependSuffix): ../../librtt/Rtt_Message.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/up_up_librtt_Rtt_Message.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/up_up_librtt_Rtt_Message.cpp$(DependSuffix) -MM ../../librtt/Rtt_Message.cpp

$(IntermediateDirectory)/up_up_librtt_Rtt_Message.cpp$(PreprocessSuffix): ../../librtt/Rtt_Message.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/up_up_librtt_Rtt_Message.cpp$(PreprocessSuffix) ../../librtt/Rtt_Message.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


