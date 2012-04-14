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
CFLAGS= -O2 -fPIC -MMD -MP

# CC Compiler Flags
CXXFLAGS= -O2 -DKOMPEX_SQLITEWRAPPER_EXPORT -DKOMPEX_SQLITEWRAPPER_DYN -fPIC -MMD -MP -I${INCLUDE_DIR}

# Link Libraries and Options
LDLIBSOPTIONS= -shared -fPIC

# Build Targets
.build-conf: ${LIB_DIR}/${PRODUCT_NAME}.so

${LIB_DIR}/${PRODUCT_NAME}.so: ${OBJECTFILES}
	${MKDIR} -p ${LIB_DIR}
	${LINK.cc} -o ${LIB_DIR}/${PRODUCT_NAME}.so ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/KompexSQLiteBlob.o: ${SRC_DIR}/KompexSQLiteBlob.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) ${CXXFLAGS} -MF $@.d -o $@ $^

${OBJECTDIR}/KompexSQLiteStatement.o: ${SRC_DIR}/KompexSQLiteStatement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) ${CXXFLAGS} -MF $@.d -o $@ $^

${OBJECTDIR}/KompexSQLiteDatabase.o: ${SRC_DIR}/KompexSQLiteDatabase.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) ${CXXFLAGS} -MF $@.d -o $@ $^

${OBJECTDIR}/sqlite3.o: ${SRC_DIR}/sqlite3.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.c) ${CFLAGS} -MF $@.d -o $@ $^

