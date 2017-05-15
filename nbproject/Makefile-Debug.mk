#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/LuaScript.o \
	${OBJECTDIR}/src/main.o

# Test Directory
TESTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}/tests

# Test Files
TESTFILES= \
	${TESTDIR}/TestFiles/f1

# Test Object Files
TESTOBJECTFILES= \
	${TESTDIR}/_ext/1996e557/LuaScriptTest.o \
	${TESTDIR}/_ext/1996e557/LuaScriptTestRunner.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=`pkg-config --libs lua5.2`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/amos-ss17-proj4

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/amos-ss17-proj4: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/amos-ss17-proj4 ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/LuaScript.o: src/LuaScript.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/lua5.2 -I/home/bauer/Projects/can-utils/include -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/LuaScript.o src/LuaScript.cpp

${OBJECTDIR}/src/main.o: src/main.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/lua5.2 -I/home/bauer/Projects/can-utils/include -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main.o src/main.cpp

# Subprojects
.build-subprojects:

# Build Test Targets
.build-tests-conf: .build-tests-subprojects .build-conf ${TESTFILES}
.build-tests-subprojects:

${TESTDIR}/TestFiles/f1: ${TESTDIR}/_ext/1996e557/LuaScriptTest.o ${TESTDIR}/_ext/1996e557/LuaScriptTestRunner.o ${OBJECTFILES:%.o=%_nomain.o}
	${MKDIR} -p ${TESTDIR}/TestFiles
	${LINK.cc} -o ${TESTDIR}/TestFiles/f1 $^ ${LDLIBSOPTIONS}   `cppunit-config --libs`   


${TESTDIR}/_ext/1996e557/LuaScriptTest.o: /home/bauer/Projects/amos-ss17-proj4/tests/LuaScriptTest.cpp 
	${MKDIR} -p ${TESTDIR}/_ext/1996e557
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/lua5.2 -I/home/bauer/Projects/can-utils/include -I/home/bauer/Projects/amos-ss17-proj4/src -std=c++14 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/_ext/1996e557/LuaScriptTest.o /home/bauer/Projects/amos-ss17-proj4/tests/LuaScriptTest.cpp


${TESTDIR}/_ext/1996e557/LuaScriptTestRunner.o: /home/bauer/Projects/amos-ss17-proj4/tests/LuaScriptTestRunner.cpp 
	${MKDIR} -p ${TESTDIR}/_ext/1996e557
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -I/usr/include/lua5.2 -I/home/bauer/Projects/can-utils/include -I/home/bauer/Projects/amos-ss17-proj4/src -std=c++14 `cppunit-config --cflags` -MMD -MP -MF "$@.d" -o ${TESTDIR}/_ext/1996e557/LuaScriptTestRunner.o /home/bauer/Projects/amos-ss17-proj4/tests/LuaScriptTestRunner.cpp


${OBJECTDIR}/src/LuaScript_nomain.o: ${OBJECTDIR}/src/LuaScript.o src/LuaScript.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/LuaScript.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/lua5.2 -I/home/bauer/Projects/can-utils/include -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/LuaScript_nomain.o src/LuaScript.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/LuaScript.o ${OBJECTDIR}/src/LuaScript_nomain.o;\
	fi

${OBJECTDIR}/src/main_nomain.o: ${OBJECTDIR}/src/main.o src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	@NMOUTPUT=`${NM} ${OBJECTDIR}/src/main.o`; \
	if (echo "$$NMOUTPUT" | ${GREP} '|main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T main$$') || \
	   (echo "$$NMOUTPUT" | ${GREP} 'T _main$$'); \
	then  \
	    ${RM} "$@.d";\
	    $(COMPILE.cc) -g -Wall -I/usr/include/lua5.2 -I/home/bauer/Projects/can-utils/include -std=c++14 -Dmain=__nomain -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/main_nomain.o src/main.cpp;\
	else  \
	    ${CP} ${OBJECTDIR}/src/main.o ${OBJECTDIR}/src/main_nomain.o;\
	fi

# Run Test Targets
.test-conf:
	@if [ "${TEST}" = "" ]; \
	then  \
	    ${TESTDIR}/TestFiles/f1 || true; \
	else  \
	    ./${TEST} || true; \
	fi

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
