TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library
WindowsTargetPlatformVersion=8.1
ifneq ($(findstring 10.0,$(VCVERSION)),)
VSNUM=2010
endif
ifneq ($(findstring 15.0,$(VCVERSION)),)
VSNUM=2017
WindowsTargetPlatformVersion=$(subst \,,$(WindowsSDKVersion))
endif
ifneq ($(findstring 16.0,$(VCVERSION)),)
VSNUM=2019
WindowsTargetPlatformVersion=$(subst \,,$(WindowsSDKVersion))
endif
SEABREEZE_TOP = $(TOP)/vendor/SeaBreeze
SEABREEZE_WINTOP = $(SEABREEZE_TOP)/os-support/windows/VisualStudio$(VSNUM)
SEABREEZE_SLN = $(SEABREEZE_WINTOP)/SeaBreeze.sln
SEABREEZE_DLL = SeaBreeze.dll
SEABREEZE_LIB = SeaBreeze.lib
SEABREEZE_PDB = SeaBreeze.pdb

MSBUILD_BIN = MSBuild.exe

VS_CONFIG=Release
VS_PLATFORM=Win32
SB_ARCH=i386
MSBUILD_OUTPUT_DIR := $(SEABREEZE_WINTOP)
ifneq ($(findstring windows,$(EPICS_HOST_ARCH)),)
VS_PLATFORM=x64
SB_ARCH=amd64
MSBUILD_OUTPUT_DIR := $(MSBUILD_OUTPUT_DIR)/x64
endif
ifneq ($(findstring debug,$(EPICS_HOST_ARCH)),)
VS_CONFIG=Debug
endif
MSBUILD_OPTS = /p:Configuration=$(VS_CONFIG) /p:Platform=$(VS_PLATFORM);WindowsTargetPlatformVersion=$(WindowsTargetPlatformVersion)
MSBUILD_OUTPUT_DIR := $(MSBUILD_OUTPUT_DIR)/$(VS_CONFIG)

# not sure if needed?
#BIN_INSTALLS += \
# $(SEABREEZE_WINTOP)\FilesToInstall\windows-support\$(SB_ARCH)\dpinst.exe \
# $(SEABREEZE_WINTOP)\FilesToInstall\windows-support\$(SB_ARCH)\WdfCoInstaller01007.dll \
# $(SEABREEZE_WINTOP)\FilesToInstall\windows-support\$(SB_ARCH)\WinUSBCoInstaller.dll \
# $(SEABREEZE_WINTOP)\FilesToInstall\windows-support\$(SB_ARCH)\WUDFUpdate_01007.dll 

include $(TOP)/configure/RULES

ifneq ($(findstring linux,$(EPICS_HOST_ARCH)),)
install:
	$(MAKE) -C $(SEABREEZE_TOP)
	-$(MKDIR) -p $(TOP)/lib/$(EPICS_HOST_ARCH) $(TOP)/bin/$(EPICS_HOST_ARCH)
	$(CP) $(SEABREEZE_TOP)/lib/libseabreeze.so $(TOP)/lib/$(EPICS_HOST_ARCH)
	$(CP) $(SEABREEZE_TOP)/lib/libseabreeze.a $(TOP)/lib/$(EPICS_HOST_ARCH)

clean:
	$(MAKE) -C $(SEABREEZE_TOP) clean
else
install:
	@echo Building seabreeze in $(MSBUILD_OUTPUT_DIR) 
	$(MSBUILD_BIN) $(MSBUILD_OPTS) $(SEABREEZE_SLN)
	-$(MKDIR) $(TOP)/lib/$(EPICS_HOST_ARCH) $(TOP)/bin/$(EPICS_HOST_ARCH)
	$(CP) $(MSBUILD_OUTPUT_DIR)/$(SEABREEZE_LIB) $(TOP)/lib/$(EPICS_HOST_ARCH)
	$(CP) $(MSBUILD_OUTPUT_DIR)/$(SEABREEZE_DLL) $(MSBUILD_OUTPUT_DIR)/$(SEABREEZE_PDB) $(TOP)/bin/$(EPICS_HOST_ARCH)

clean:
	$(MSBUILD_BIN) $(MSBUILD_OPTS) /t:Clean $(SEABREEZE_SLN)
	$(RMDIR) $(SEABREEZE_WINTOP)/Debug $(SEABREEZE_WINTOP)/Release $(SEABREEZE_WINTOP)/x64
	$(RMDIR) $(SEABREEZE_WINTOP)/VSProj/Debug $(SEABREEZE_WINTOP)/VSProj/Release $(SEABREEZE_WINTOP)/VSProj/x64
endif
