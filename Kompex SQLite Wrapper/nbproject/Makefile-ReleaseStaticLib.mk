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
CND_CONF=ReleaseStaticLib
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${BUILD_DIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/KompexSQLiteBlob.o \
	${OBJECTDIR}/KompexSQLiteStatement.o \
	${OBJECTDIR}/KompexSQLiteDatabase.o \
	${OBJECTDIR}/sqlite3.o

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
	${MAKE}  -f nbproject/Makefile-ReleaseStaticLib.mk ${LIB_DIR}/release/KompexSQLiteWrapper_Static.a

${LIB_DIR}/release/KompexSQLiteWrapper_Static.a: ${OBJECTFILES}
	${MKDIR} -p ${LIB_DIR}/release
	${RM} ${LIB_DIR}/release/KompexSQLiteWrapper_Static.a
	${AR} rv ${LIB_DIR}/release/KompexSQLiteWrapper_Static.a ${OBJECTFILES} 
	$(RANLIB) ${LIB_DIR}/release/KompexSQLiteWrapper_Static.a

${OBJECTDIR}/KompexSQLiteBlob.o: nbproject/Makefile-${CND_CONF}.mk ${SRC_DIR}/KompexSQLiteBlob.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I${INCLUDE_DIR} -MMD -MP -MF $@.d -o ${OBJECTDIR}/KompexSQLiteBlob.o ${SRC_DIR}/KompexSQLiteBlob.cpp

${OBJECTDIR}/KompexSQLiteStatement.o: nbproject/Makefile-${CND_CONF}.mk ${SRC_DIR}/KompexSQLiteStatement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I${INCLUDE_DIR} -MMD -MP -MF $@.d -o ${OBJECTDIR}/KompexSQLiteStatement.o ${SRC_DIR}/KompexSQLiteStatement.cpp

${OBJECTDIR}/KompexSQLiteDatabase.o: nbproject/Makefile-${CND_CONF}.mk ${SRC_DIR}/KompexSQLiteDatabase.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I${INCLUDE_DIR} -MMD -MP -MF $@.d -o ${OBJECTDIR}/KompexSQLiteDatabase.o ${SRC_DIR}/KompexSQLiteDatabase.cpp

${OBJECTDIR}/sqlite3.o: nbproject/Makefile-${CND_CONF}.mk ${SRC_DIR}/sqlite3.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sqlite3.o ${SRC_DIR}/sqlite3.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/ReleaseStaticLib
	${RM} ${LIB_DIR}/release/KompexSQLiteWrapper_Static.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
