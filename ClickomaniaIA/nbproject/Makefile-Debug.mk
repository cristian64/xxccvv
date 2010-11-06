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
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/NodeInterface.o \
	${OBJECTDIR}/algorithms.o \
	${OBJECTDIR}/Board.o


# C Compiler Flags
CFLAGS=-pg

# CC Compiler Flags
CCFLAGS=-pg
CXXFLAGS=-pg

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/clickomaniaia

dist/Debug/GNU-Linux-x86/clickomaniaia: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -pg -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clickomaniaia ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/NodeInterface.o: nbproject/Makefile-${CND_CONF}.mk NodeInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/NodeInterface.o NodeInterface.cpp

${OBJECTDIR}/algorithms.o: nbproject/Makefile-${CND_CONF}.mk algorithms.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/algorithms.o algorithms.cpp

${OBJECTDIR}/Board.o: nbproject/Makefile-${CND_CONF}.mk Board.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/Board.o Board.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/clickomaniaia

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
