#!/bin/bash -x

#
# Generated - do not edit!
#

# Macros
TOP=`pwd`
CND_PLATFORM=GNU-Linux-x86
CND_CONF=DebugDynamicLib
CND_DISTDIR=dist
TMPDIR=build/${CND_CONF}/${CND_PLATFORM}/tmp-packaging
TMPDIRNAME=tmp-packaging
OUTPUT_PATH=../lib/debug/KompexSQLiteWrapper_d.so
OUTPUT_BASENAME=KompexSQLiteWrapper_d.so
PACKAGE_TOP_DIR=debug/

# Functions
function checkReturnCode
{
    rc=$?
    if [ $rc != 0 ]
    then
        exit $rc
    fi
}
function makeDirectory
# $1 directory path
# $2 permission (optional)
{
    mkdir -p "$1"
    checkReturnCode
    if [ "$2" != "" ]
    then
      chmod $2 "$1"
      checkReturnCode
    fi
}
function copyFileToTmpDir
# $1 from-file path
# $2 to-file path
# $3 permission
{
    cp "$1" "$2"
    checkReturnCode
    if [ "$3" != "" ]
    then
        chmod $3 "$2"
        checkReturnCode
    fi
}

# Setup
cd "${TOP}"
mkdir -p ../lib
rm -rf ${TMPDIR}
mkdir -p ${TMPDIR}

# Copy files and create directories and links
cd "${TOP}"
makeDirectory ${TMPDIR}/../lib/debug
copyFileToTmpDir "${OUTPUT_PATH}" "${TMPDIR}/${OUTPUT_PATH}" 0644


# Generate tar file
cd "${TOP}"
rm -f ../lib/debug
cd ${TMPDIR}
tar -vcf ../../../../../lib/debug *
checkReturnCode

# Cleanup
cd "${TOP}"
rm -rf ${TMPDIR}
