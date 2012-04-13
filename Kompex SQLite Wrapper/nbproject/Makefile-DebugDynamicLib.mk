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
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=DebugDynamicLib
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/_DOTDOT/src/KompexSQLiteBlob.o \
	${OBJECTDIR}/_ext/_DOTDOT/src/KompexSQLiteStatement.o \
	${OBJECTDIR}/_ext/_DOTDOT/src/KompexSQLiteDatabase.o \
	${OBJECTDIR}/_ext/_DOTDOT/src/sqlite3.o

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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-DebugDynamicLib.mk ../lib/debug/KompexSQLiteWrapper_d.so

../lib/debug/KompexSQLiteWrapper_d.so: ${OBJECTFILES}
	${MKDIR} -p ../lib/debug
	${LINK.cc} -shared -o ../lib/debug/KompexSQLiteWrapper_d.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/src/KompexSQLiteBlob.o: nbproject/Makefile-${CND_CONF}.mk ../src/KompexSQLiteBlob.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/src
	${RM} $@.d
	$(COMPILE.cc) -g -DKOMPEX_SQLITEWRAPPER_EXPORT -DKOMPEX_SQLITEWRAPPER_DYN -I../inc -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/src/KompexSQLiteBlob.o ../src/KompexSQLiteBlob.cpp

${OBJECTDIR}/_ext/_DOTDOT/src/KompexSQLiteStatement.o: nbproject/Makefile-${CND_CONF}.mk ../src/KompexSQLiteStatement.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/src
	${RM} $@.d
	$(COMPILE.cc) -g -DKOMPEX_SQLITEWRAPPER_EXPORT -DKOMPEX_SQLITEWRAPPER_DYN -I../inc -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/src/KompexSQLiteStatement.o ../src/KompexSQLiteStatement.cpp

${OBJECTDIR}/_ext/_DOTDOT/src/KompexSQLiteDatabase.o: nbproject/Makefile-${CND_CONF}.mk ../src/KompexSQLiteDatabase.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/src
	${RM} $@.d
	$(COMPILE.cc) -g -DKOMPEX_SQLITEWRAPPER_EXPORT -DKOMPEX_SQLITEWRAPPER_DYN -I../inc -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/src/KompexSQLiteDatabase.o ../src/KompexSQLiteDatabase.cpp

${OBJECTDIR}/_ext/_DOTDOT/src/sqlite3.o: nbproject/Makefile-${CND_CONF}.mk ../src/sqlite3.c 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/src
	${RM} $@.d
	$(COMPILE.c) -g -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/src/sqlite3.o ../src/sqlite3.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/DebugDynamicLib
	${RM} ../lib/debug/KompexSQLiteWrapper_d.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
