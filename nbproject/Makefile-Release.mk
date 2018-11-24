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
	${OBJECTDIR}/src/dfc/dfc3DEntity.o \
	${OBJECTDIR}/src/dfc/dfc3DMaze.o \
	${OBJECTDIR}/src/dfc/dfc3DSprite.o \
	${OBJECTDIR}/src/dfc/dfcApp.o \
	${OBJECTDIR}/src/dfc/dfcBackground.o \
	${OBJECTDIR}/src/dfc/dfcButton.o \
	${OBJECTDIR}/src/dfc/dfcCamera.o \
	${OBJECTDIR}/src/dfc/dfcContext.o \
	${OBJECTDIR}/src/dfc/dfcDictionary.o \
	${OBJECTDIR}/src/dfc/dfcEntity.o \
	${OBJECTDIR}/src/dfc/dfcFont.o \
	${OBJECTDIR}/src/dfc/dfcLabel.o \
	${OBJECTDIR}/src/dfc/dfcMusic.o \
	${OBJECTDIR}/src/dfc/dfcObj.o \
	${OBJECTDIR}/src/dfc/dfcPhysObj.o \
	${OBJECTDIR}/src/dfc/dfcRPGEngine.o \
	${OBJECTDIR}/src/dfc/dfcRPGEngineEntities.o \
	${OBJECTDIR}/src/dfc/dfcRPGEngineItems.o \
	${OBJECTDIR}/src/dfc/dfcRPGEngineSpells.o \
	${OBJECTDIR}/src/dfc/dfcSceneEngine.o \
	${OBJECTDIR}/src/dfc/dfcSceneNodes.o \
	${OBJECTDIR}/src/dfc/dfcSound.o \
	${OBJECTDIR}/src/helper/DString.o \
	${OBJECTDIR}/src/helper/XMLLoader.o \
	${OBJECTDIR}/src/lib/libMath.o \
	${OBJECTDIR}/src/lib/libMngIO.o \
	${OBJECTDIR}/src/lib/libMngResIO.o \
	${OBJECTDIR}/src/lib/linux/libMng3D_linux.o \
	${OBJECTDIR}/src/lib/linux/libMngGraphic_linux.o \
	${OBJECTDIR}/src/lib/linux/libMngInput_linux.o \
	${OBJECTDIR}/src/lib/linux/libMngSound_linux.o \
	${OBJECTDIR}/src/lib/linux/libSystem_linux.o \
	${OBJECTDIR}/src/newClass.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdakiilib.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdakiilib.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdakiilib.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdakiilib.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libdakiilib.a

${OBJECTDIR}/src/dfc/dfc3DEntity.o: src/dfc/dfc3DEntity.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfc3DEntity.o src/dfc/dfc3DEntity.cpp

${OBJECTDIR}/src/dfc/dfc3DMaze.o: src/dfc/dfc3DMaze.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfc3DMaze.o src/dfc/dfc3DMaze.cpp

${OBJECTDIR}/src/dfc/dfc3DSprite.o: src/dfc/dfc3DSprite.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfc3DSprite.o src/dfc/dfc3DSprite.cpp

${OBJECTDIR}/src/dfc/dfcApp.o: src/dfc/dfcApp.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcApp.o src/dfc/dfcApp.cpp

${OBJECTDIR}/src/dfc/dfcBackground.o: src/dfc/dfcBackground.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcBackground.o src/dfc/dfcBackground.cpp

${OBJECTDIR}/src/dfc/dfcButton.o: src/dfc/dfcButton.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcButton.o src/dfc/dfcButton.cpp

${OBJECTDIR}/src/dfc/dfcCamera.o: src/dfc/dfcCamera.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcCamera.o src/dfc/dfcCamera.cpp

${OBJECTDIR}/src/dfc/dfcContext.o: src/dfc/dfcContext.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcContext.o src/dfc/dfcContext.cpp

${OBJECTDIR}/src/dfc/dfcDictionary.o: src/dfc/dfcDictionary.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcDictionary.o src/dfc/dfcDictionary.cpp

${OBJECTDIR}/src/dfc/dfcEntity.o: src/dfc/dfcEntity.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcEntity.o src/dfc/dfcEntity.cpp

${OBJECTDIR}/src/dfc/dfcFont.o: src/dfc/dfcFont.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcFont.o src/dfc/dfcFont.cpp

${OBJECTDIR}/src/dfc/dfcLabel.o: src/dfc/dfcLabel.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcLabel.o src/dfc/dfcLabel.cpp

${OBJECTDIR}/src/dfc/dfcMusic.o: src/dfc/dfcMusic.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcMusic.o src/dfc/dfcMusic.cpp

${OBJECTDIR}/src/dfc/dfcObj.o: src/dfc/dfcObj.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcObj.o src/dfc/dfcObj.cpp

${OBJECTDIR}/src/dfc/dfcPhysObj.o: src/dfc/dfcPhysObj.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcPhysObj.o src/dfc/dfcPhysObj.cpp

${OBJECTDIR}/src/dfc/dfcRPGEngine.o: src/dfc/dfcRPGEngine.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcRPGEngine.o src/dfc/dfcRPGEngine.cpp

${OBJECTDIR}/src/dfc/dfcRPGEngineEntities.o: src/dfc/dfcRPGEngineEntities.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcRPGEngineEntities.o src/dfc/dfcRPGEngineEntities.cpp

${OBJECTDIR}/src/dfc/dfcRPGEngineItems.o: src/dfc/dfcRPGEngineItems.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcRPGEngineItems.o src/dfc/dfcRPGEngineItems.cpp

${OBJECTDIR}/src/dfc/dfcRPGEngineSpells.o: src/dfc/dfcRPGEngineSpells.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcRPGEngineSpells.o src/dfc/dfcRPGEngineSpells.cpp

${OBJECTDIR}/src/dfc/dfcSceneEngine.o: src/dfc/dfcSceneEngine.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcSceneEngine.o src/dfc/dfcSceneEngine.cpp

${OBJECTDIR}/src/dfc/dfcSceneNodes.o: src/dfc/dfcSceneNodes.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcSceneNodes.o src/dfc/dfcSceneNodes.cpp

${OBJECTDIR}/src/dfc/dfcSound.o: src/dfc/dfcSound.cpp
	${MKDIR} -p ${OBJECTDIR}/src/dfc
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/dfc/dfcSound.o src/dfc/dfcSound.cpp

${OBJECTDIR}/src/helper/DString.o: src/helper/DString.cpp
	${MKDIR} -p ${OBJECTDIR}/src/helper
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/helper/DString.o src/helper/DString.cpp

${OBJECTDIR}/src/helper/XMLLoader.o: src/helper/XMLLoader.cpp
	${MKDIR} -p ${OBJECTDIR}/src/helper
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/helper/XMLLoader.o src/helper/XMLLoader.cpp

${OBJECTDIR}/src/lib/libMath.o: src/lib/libMath.cpp
	${MKDIR} -p ${OBJECTDIR}/src/lib
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lib/libMath.o src/lib/libMath.cpp

${OBJECTDIR}/src/lib/libMngIO.o: src/lib/libMngIO.cpp
	${MKDIR} -p ${OBJECTDIR}/src/lib
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lib/libMngIO.o src/lib/libMngIO.cpp

${OBJECTDIR}/src/lib/libMngResIO.o: src/lib/libMngResIO.cpp
	${MKDIR} -p ${OBJECTDIR}/src/lib
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lib/libMngResIO.o src/lib/libMngResIO.cpp

${OBJECTDIR}/src/lib/linux/libMng3D_linux.o: src/lib/linux/libMng3D_linux.cpp
	${MKDIR} -p ${OBJECTDIR}/src/lib/linux
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lib/linux/libMng3D_linux.o src/lib/linux/libMng3D_linux.cpp

${OBJECTDIR}/src/lib/linux/libMngGraphic_linux.o: src/lib/linux/libMngGraphic_linux.cpp
	${MKDIR} -p ${OBJECTDIR}/src/lib/linux
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lib/linux/libMngGraphic_linux.o src/lib/linux/libMngGraphic_linux.cpp

${OBJECTDIR}/src/lib/linux/libMngInput_linux.o: src/lib/linux/libMngInput_linux.cpp
	${MKDIR} -p ${OBJECTDIR}/src/lib/linux
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lib/linux/libMngInput_linux.o src/lib/linux/libMngInput_linux.cpp

${OBJECTDIR}/src/lib/linux/libMngSound_linux.o: src/lib/linux/libMngSound_linux.cpp
	${MKDIR} -p ${OBJECTDIR}/src/lib/linux
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lib/linux/libMngSound_linux.o src/lib/linux/libMngSound_linux.cpp

${OBJECTDIR}/src/lib/linux/libSystem_linux.o: src/lib/linux/libSystem_linux.cpp
	${MKDIR} -p ${OBJECTDIR}/src/lib/linux
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/lib/linux/libSystem_linux.o src/lib/linux/libSystem_linux.cpp

${OBJECTDIR}/src/newClass.o: src/newClass.cpp
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/newClass.o src/newClass.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
