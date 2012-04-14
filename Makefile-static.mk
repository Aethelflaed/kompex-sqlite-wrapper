# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${BUILD_DIR}/${CONF}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/KompexSQLiteBlob.o \
	${OBJECTDIR}/KompexSQLiteStatement.o \
	${OBJECTDIR}/KompexSQLiteDatabase.o \
	${OBJECTDIR}/sqlite3.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CXXFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf:
	${MAKE}  -f Makefile-${CONF}.mk ${LIB_DIR}/${PRODUCT_NAME}.a

${LIB_DIR}/${PRODUCT_NAME}.a: ${OBJECTFILES}
	${MKDIR} -p ${LIB_DIR}
	${RM} ${LIB_DIR}/${PRODUCT_NAME}.a
	${AR} rv ${LIB_DIR}/${PRODUCT_NAME}.a ${OBJECTFILES} 
	$(RANLIB) ${LIB_DIR}/${PRODUCT_NAME}.a

${OBJECTDIR}/KompexSQLiteBlob.o: Makefile-${CONF}.mk ${SRC_DIR}/KompexSQLiteBlob.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I${INCLUDE_DIR} -MMD -MP -MF $@.d -o ${OBJECTDIR}/KompexSQLiteBlob.o ${SRC_DIR}/KompexSQLiteBlob.cpp

${OBJECTDIR}/KompexSQLiteStatement.o: Makefile-${CONF}.mk ${SRC_DIR}/KompexSQLiteStatement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I${INCLUDE_DIR} -MMD -MP -MF $@.d -o ${OBJECTDIR}/KompexSQLiteStatement.o ${SRC_DIR}/KompexSQLiteStatement.cpp

${OBJECTDIR}/KompexSQLiteDatabase.o: Makefile-${CONF}.mk ${SRC_DIR}/KompexSQLiteDatabase.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I${INCLUDE_DIR} -MMD -MP -MF $@.d -o ${OBJECTDIR}/KompexSQLiteDatabase.o ${SRC_DIR}/KompexSQLiteDatabase.cpp

${OBJECTDIR}/sqlite3.o: Makefile-${CONF}.mk ${SRC_DIR}/sqlite3.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/sqlite3.o ${SRC_DIR}/sqlite3.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/${CONF}
	${RM} ${LIB_DIR}/${PRODUCT_NAME}.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
