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
CFLAGS= -O2 -MMD -MP

# CC Compiler Flags
CXXFLAGS= -O2 -I${INCLUDE_DIR} -MMD -MP

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf:${LIB_DIR}/${PRODUCT_NAME}.a

${LIB_DIR}/${PRODUCT_NAME}.a: ${OBJECTFILES}
	${MKDIR} -p ${LIB_DIR}
	${RM} ${LIB_DIR}/${PRODUCT_NAME}.a
	${AR} rv ${LIB_DIR}/${PRODUCT_NAME}.a ${OBJECTFILES} 
	$(RANLIB) ${LIB_DIR}/${PRODUCT_NAME}.a

${OBJECTDIR}/KompexSQLiteBlob.o: ${SRC_DIR}/KompexSQLiteBlob.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -MF $@.d -o $@ $^

${OBJECTDIR}/KompexSQLiteStatement.o: ${SRC_DIR}/KompexSQLiteStatement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -MF $@.d -o $@ $^

${OBJECTDIR}/KompexSQLiteDatabase.o: ${SRC_DIR}/KompexSQLiteDatabase.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -MF $@.d -o $@ $^

${OBJECTDIR}/sqlite3.o: ${SRC_DIR}/sqlite3.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) ${CFLAGS} -MF $@.d -o $@ $^

