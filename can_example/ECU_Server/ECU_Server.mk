##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Release
ProjectName            :=ECU_Server
ConfigurationName      :=Release
WorkspacePath          :=/home/cloud/IT-Projekt
ProjectPath            :=/home/cloud/IT-Projekt/ECU
IntermediateDirectory  :=./Release
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=cloud
Date                   :=23/05/17
CodeLitePath           :=/home/cloud/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=$(PreprocessorSwitch)NDEBUG 
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="ECU_Server.txt"
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
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -O2 -Wall $(Preprocessors)
CFLAGS   :=  -O2 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/CAN_UDS_Layer.cpp$(ObjectSuffix) $(IntermediateDirectory)/SocketCAN.cpp$(ObjectSuffix) $(IntermediateDirectory)/UDS_ECU.cpp$(ObjectSuffix) 



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
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/cloud/IT-Projekt/ECU/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM main.cpp

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) main.cpp

$(IntermediateDirectory)/CAN_UDS_Layer.cpp$(ObjectSuffix): CAN_UDS_Layer.cpp $(IntermediateDirectory)/CAN_UDS_Layer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/cloud/IT-Projekt/ECU/CAN_UDS_Layer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/CAN_UDS_Layer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/CAN_UDS_Layer.cpp$(DependSuffix): CAN_UDS_Layer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/CAN_UDS_Layer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/CAN_UDS_Layer.cpp$(DependSuffix) -MM CAN_UDS_Layer.cpp

$(IntermediateDirectory)/CAN_UDS_Layer.cpp$(PreprocessSuffix): CAN_UDS_Layer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/CAN_UDS_Layer.cpp$(PreprocessSuffix) CAN_UDS_Layer.cpp

$(IntermediateDirectory)/SocketCAN.cpp$(ObjectSuffix): SocketCAN.cpp $(IntermediateDirectory)/SocketCAN.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/cloud/IT-Projekt/ECU/SocketCAN.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/SocketCAN.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/SocketCAN.cpp$(DependSuffix): SocketCAN.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/SocketCAN.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/SocketCAN.cpp$(DependSuffix) -MM SocketCAN.cpp

$(IntermediateDirectory)/SocketCAN.cpp$(PreprocessSuffix): SocketCAN.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/SocketCAN.cpp$(PreprocessSuffix) SocketCAN.cpp

$(IntermediateDirectory)/UDS_ECU.cpp$(ObjectSuffix): UDS_ECU.cpp $(IntermediateDirectory)/UDS_ECU.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/cloud/IT-Projekt/ECU/UDS_ECU.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/UDS_ECU.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/UDS_ECU.cpp$(DependSuffix): UDS_ECU.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/UDS_ECU.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/UDS_ECU.cpp$(DependSuffix) -MM UDS_ECU.cpp

$(IntermediateDirectory)/UDS_ECU.cpp$(PreprocessSuffix): UDS_ECU.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/UDS_ECU.cpp$(PreprocessSuffix) UDS_ECU.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Release/


