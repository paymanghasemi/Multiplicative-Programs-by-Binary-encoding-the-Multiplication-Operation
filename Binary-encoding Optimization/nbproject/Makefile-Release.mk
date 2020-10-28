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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Sources/Binary_Reformulation.o \
	${OBJECTDIR}/Sources/Multiplication.o \
	${OBJECTDIR}/Sources/Optimizations.o \
	${OBJECTDIR}/Sources/Reading_LP_file.o \
	${OBJECTDIR}/Sources/Solution_Vector.o \
	${OBJECTDIR}/Sources/Write_Output.o \
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
LDLIBSOPTIONS=-L/home/p/payman/Gurobi/gurobi902/linux64/lib /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi_g++5.2.a /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi_c++.a /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi90.so -lm -lpthread -ldl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk BRF_GRB

BRF_GRB: /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi_g++5.2.a

BRF_GRB: /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi_c++.a

BRF_GRB: /home/p/payman/Gurobi/gurobi902/linux64/lib/libgurobi90.so

BRF_GRB: ${OBJECTFILES}
	${LINK.cc} -o BRF_GRB ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Sources/Binary_Reformulation.o: Sources/Binary_Reformulation.cpp
	${MKDIR} -p ${OBJECTDIR}/Sources
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -w -DIL_STD -I/home/p/payman/Gurobi/gurobi902/linux64/include -IHeaders -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Sources/Binary_Reformulation.o Sources/Binary_Reformulation.cpp

${OBJECTDIR}/Sources/Multiplication.o: Sources/Multiplication.cpp
	${MKDIR} -p ${OBJECTDIR}/Sources
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -w -DIL_STD -I/home/p/payman/Gurobi/gurobi902/linux64/include -IHeaders -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Sources/Multiplication.o Sources/Multiplication.cpp

${OBJECTDIR}/Sources/Optimizations.o: Sources/Optimizations.cpp
	${MKDIR} -p ${OBJECTDIR}/Sources
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -w -DIL_STD -I/home/p/payman/Gurobi/gurobi902/linux64/include -IHeaders -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Sources/Optimizations.o Sources/Optimizations.cpp

${OBJECTDIR}/Sources/Reading_LP_file.o: Sources/Reading_LP_file.cpp
	${MKDIR} -p ${OBJECTDIR}/Sources
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -w -DIL_STD -I/home/p/payman/Gurobi/gurobi902/linux64/include -IHeaders -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Sources/Reading_LP_file.o Sources/Reading_LP_file.cpp

${OBJECTDIR}/Sources/Solution_Vector.o: Sources/Solution_Vector.cpp
	${MKDIR} -p ${OBJECTDIR}/Sources
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -w -DIL_STD -I/home/p/payman/Gurobi/gurobi902/linux64/include -IHeaders -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Sources/Solution_Vector.o Sources/Solution_Vector.cpp

${OBJECTDIR}/Sources/Write_Output.o: Sources/Write_Output.cpp
	${MKDIR} -p ${OBJECTDIR}/Sources
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -w -DIL_STD -I/home/p/payman/Gurobi/gurobi902/linux64/include -IHeaders -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Sources/Write_Output.o Sources/Write_Output.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -w -DIL_STD -I/home/p/payman/Gurobi/gurobi902/linux64/include -IHeaders -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} -r null/libgurobi90.so
	${RM} BRF_GRB

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
