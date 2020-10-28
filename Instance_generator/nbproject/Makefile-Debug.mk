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
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=/home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi_g++5.2.a /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi_c++.a /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi90.so -lm -lpthread -ldl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk Instance_Generator

Instance_Generator: /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi_g++5.2.a

Instance_Generator: /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi_c++.a

Instance_Generator: /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi90.so

Instance_Generator: ${OBJECTFILES}
	${LINK.cc} -o Instance_Generator ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DIL_STD -I/home/p/payman/Gurobi/gurobi902/linux64/include -IHeaders -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r null/libgurobi90.so
	${RM} Instance_Generator

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
