# Environment 
MKDIR=mkdir
CP=cp
RANLIB=ranlib
CC=gcc
CXX=g++
AS=as

SRC_DIR=src
INCLUDE_DIR=include
LIB_DIR=lib
BUILD_DIR=build

PLATFORM=GNU-Linux
PROJECTNAME=Kompex SQLite Wrapper
PRODUCT_NAME=kompex-sqlite-wrapper

DEFAULTCONF=shared
CONF=${DEFAULTCONF}

ALLCONFS=static shared


build: .validate-impl .depcheck-impl
	@#echo "=> Running $@... Configuration=$(CONF)"
	${MAKE} -f Makefile-${CONF}.mk .build-conf


clean: .validate-impl .depcheck-impl
	@#echo "=> Running $@... Configuration=$(CONF)"
	${MAKE} -f Makefile-${CONF}.mk .clean-conf

clobber: .depcheck-impl
	@#echo "=> Running $@..."
	for CONF in ${ALLCONFS}; \
	do \
	    ${MAKE} -f Makefile-$${CONF}.mk .clean-conf; \
	done

all: .depcheck-impl
	@#echo "=> Running $@..."
	for CONF in ${ALLCONFS}; \
	do \
	    ${MAKE} -f Makefile-$${CONF}.mk CONF=$${CONF} .build-conf; \
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
	    exit 1; \
	fi

help:
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
	@echo "    make [CONF=<CONFIGURATION>] build"
	@echo "    make [CONF=<CONFIGURATION>] clean"
	@echo "    make clobber"
	@echo "    make all"
	@echo "    make help"
	@echo ""
