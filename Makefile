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

SRC_DIR=src
INCLUDE_DIR=include
LIB_DIR=lib
BUILD_DIR=build

PLATFORM=GNU-Linux
PROJECTNAME=Kompex SQLite Wrapper

DEFAULTCONF=DebugStaticLib
CONF=${DEFAULTCONF}

ALLCONFS=DebugStaticLib ReleaseStaticLib ReleaseDynamicLib DebugDynamicLib 





# build
build: .build-post

.build-pre:
# Add your pre 'build' code here...

.build-post: .build-impl
# Add your post 'build' code here...

.build-impl: .build-pre .validate-impl .depcheck-impl
	@#echo "=> Running $@... Configuration=$(CONF)"
	${MAKE} -f Makefile-${CONF}.mk SUBPROJECTS=${SUBPROJECTS} .build-conf


# clean
clean: .clean-post

.clean-pre:
# Add your pre 'clean' code here...

.clean-post: .clean-impl
# Add your post 'clean' code here...
.clean-impl: .clean-pre .validate-impl .depcheck-impl
	@#echo "=> Running $@... Configuration=$(CONF)"
	${MAKE} -f Makefile-${CONF}.mk SUBPROJECTS=${SUBPROJECTS} .clean-conf


# clobber
clobber: .clobber-post

.clobber-pre:
# Add your pre 'clobber' code here...

.clobber-post: .clobber-impl
# Add your post 'clobber' code here...
.clobber-impl: .clobber-pre .depcheck-impl
	@#echo "=> Running $@..."
	for CONF in ${ALLCONFS}; \
	do \
	    ${MAKE} -f Makefile-$${CONF}.mk SUBPROJECTS=${SUBPROJECTS} .clean-conf; \
	done


# all
all: .all-post

.all-pre:
# Add your pre 'all' code here...

.all-post: .all-impl
# Add your post 'all' code here...
.all-impl: .all-pre .depcheck-impl
	@#echo "=> Running $@..."
	for CONF in ${ALLCONFS}; \
	do \
	    ${MAKE} -f Makefile-$${CONF}.mk CONF=$${CONF} SUBPROJECTS=${SUBPROJECTS} .build-conf; \
	done

.depcheck-impl:
	@echo "# This code depends on make tool being used" >.dep.inc
	@if [ -n "${MAKE_VERSION}" ]; then \
	    echo "DEPFILES=\$$(wildcard \$$(addsuffix .d, \$${OBJECTFILES}))" >>.dep.inc; \
	    echo "ifneq (\$${DEPFILES},)" >>.dep.inc; \
	    echo "include \$${DEPFILES}" >>.dep.inc; \
	    echo "endif" >>.dep.inc; \
	else \
	    echo ".KEEP_STATE:" >>.dep.inc; \
	    echo ".KEEP_STATE_FILE:.make.state.\$${CONF}" >>.dep.inc; \
	fi

.validate-impl:
	@if [ ! -f Makefile-${CONF}.mk ]; \
	then \
	    echo ""; \
	    echo "Error: can not find the makefile for configuration '${CONF}' in project ${PROJECTNAME}"; \
	    echo "See 'make help' for details."; \
	    echo "Current directory: " `pwd`; \
	    echo ""; \
	fi
	@if [ ! -f Makefile-${CONF}.mk ]; \
	then \
	    exit 1; \
	fi

# configuration validation

# help
help: .help-post

.help-pre:
# Add your pre 'help' code here...

.help-post: .help-impl
# Add your post 'help' code here...

.help-impl: .help-pre
	@echo "This makefile supports the following configurations:"
	@echo "    ${ALLCONFS}"
	@echo ""
	@echo "and the following targets:"
	@echo "    build  (default target)"
	@echo "    clean"
	@echo "    clobber"
	@echo "    all"
	@echo "    help"
	@echo ""
	@echo "Makefile Usage:"
	@echo "    make [CONF=<CONFIGURATION>] [SUB=no] build"
	@echo "    make [CONF=<CONFIGURATION>] [SUB=no] clean"
	@echo "    make [SUB=no] clobber"
	@echo "    make [SUB=no] all"
	@echo "    make help"
	@echo ""
	@echo "Target 'build' will build a specific configuration and, unless 'SUB=no',"
	@echo "    also build subprojects."
	@echo "Target 'clean' will clean a specific configuration and, unless 'SUB=no',"
	@echo "    also clean subprojects."
	@echo "Target 'clobber' will remove all built files from all configurations and,"
	@echo "    unless 'SUB=no', also from subprojects."
	@echo "Target 'all' will will build all configurations and, unless 'SUB=no',"
	@echo "    also build subprojects."
	@echo "Target 'help' prints this message."
	@echo ""

