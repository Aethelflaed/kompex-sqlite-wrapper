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

all: static shared

clean:
	${RM} -rf build/
	${RM} -rf ${LIB_DIR}/

static:
	$(MAKE) -f Makefile-static.mk CONF=static .build-conf

shared:
	$(MAKE) -f Makefile-shared.mk CONF=shared .build-conf

