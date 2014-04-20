.SILENT:

# Makefile for citymapsengine

#BUILD_TARGET = Debug
BUILD_TARGET	= Release

######################
# Commands
######################

CD          = cd
COPY        = cp
COPYDIR     = cp -r
MAKEDIR     = mkdir -p
MOVE        = mv -f
DELETE      = rm
DELETEDIR   = rm -r
ECHO        = echo

######################
# Local Vars
######################

#Additional directories for Make to look for stuff, other than the current directory.
VPATH	= ./$(BUILD_TARGET)

#Not entirely sure, but its habit.
.SUFFIXES:  .c .o .cpp .mm

#This is the current directory. The makefile should be ran in the directory it is located.
LOCAL_PATH	= ${CURDIR}

#This is the name of the lib file to create.
EXEC_FILE	= libParticleToy.so

# This is a list of all of our .cpp files. It ignores all source files located in the Lib directory.
# Since this is for android building, it also ignores all iOS directories.
CPP_SRC_FILES	= $(shell cd $(LOCAL_PATH); find . -name "*.cpp") 

# This is a list of all of our .c files. If we had any.
C_SRC_FILES	= $(shell cd $(LOCAL_PATH); find . -name "*.c")

# This is a compilation of all of our source files that must be built into objects.
SRC_FILES	= $(CPP_SRC_FILES) $(C_SRC_FILES)

# This is a list of all of our dependency targets. They are used to control the build process.
DEPEND_FILES = $(CPP_SRC_FILES:.cpp=.d) $(C_SRC_FILES:.c=.d)

# This is a list of all of our object targets. These will be linked together to form the library.
OBJS	= $(CPP_SRC_FILES:.cpp=.o) $(C_SRC_FILES:.c=.o)

#######################
# Asset Copying Vars
#######################

#The path Android will be expecting assets.
ANDROID_ASSET_PATH := $(ANDROID_PATH)/res/raw/

#The various paths that contain our engine assets
GLSL_ASSET_PATH := $(LOCAL_PATH)/Shaders/GLSL/
CONFIG_ASSET_PATH := $(LOCAL_PATH)/Config/
CORE_CONFIG_ASSET_PATH := $(LOCAL_PATH)/Core/Config/
CITYMAPS_CONFIG_ASSET_PATH := $(LOCAL_PATH)/Citymaps/Config/

#The path Android will be expecting our lib
ANDROID_LIB_PATH := $(ANDROID_PATH)/libs/armeabi/

#The lib file
ENGINE_LIB_FILE := $(LOCAL_PATH)/$(BUILD_TARGET)/$(EXEC_FILE)

#######################
# Compiler Variables
#######################

CC	:= arm-linux-androideabi-g++
CCPP	:= arm-linux-androideabi-g++



INCDIRS	:= -I$(LOCAL_PATH) \
-I$(LOCAL_PATH)/Lib/angelscript \
-I$(CM_LIB_DIR) \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib/png \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib/freetype2/include \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib/poco/Net/include/ \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib/poco/Foundation/include/ \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib/poco/Util/include/ \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib/poco/Crypto/include/ \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib/xz/src/liblzma/api/ \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib/rapidjson/ \
-I$(CM_LIB_DIR)/CitymapsEngine/Lib/openssl/include/ \
-I$(NDK_SYSROOT)/usr/include \
-I$(NDK_CPP_ROOT)/include 

LIBS	:= -lpng-android \
-ljpegturbo-android \
-lfreetype-android \
-lPocoNet-android \
-lPocoFoundation-android  \
-lPocoUtil-android \
-lPocoCrypto-android \
-llzma-android \
-larchive-android \
-lssl-android \
-lcrypto-android \
-langelscript-android \
-lMapEngine \
-lz \
-lGLESv2 \
-lstdc++ \
-lgnustl_static \
-landroid \
-llog \
-lm \
-lc

LIBDIRS	:= -L$(LOCAL_PATH)/Lib \
-L$(CM_LIB_DIR)/CitymapsEngine/Lib/ \
-L$(CM_ENGINE_LIB_DIR) \
-L$(NDK_SYSROOT)/usr/lib \
-L$(NDK_CPP_ROOT)/libs/armeabi

LDFLAGS	:= -fPIC -shared --sysroot=$(NDK_SYSROOT)

OPTIMIZATIONS	:= -Ofast

CFLAGS  := -w $(INCDIRS) -std=c++11 -mcpu=cortex-a9 -mfpu=neon-fp16 -fPIC -DPOCO_ANDROID -DGLM_FORCE_RADIANS -frtti -fexceptions -v
CPPFLAGS	:= -w $(INCDIRS) -std=c++11 -fPIC -mcpu=cortex-a9 -mfpu=neon-fp16 -DGLM_FORCE_RADIANS -frtti -fexceptions
 
ifeq ($(BUILD_TARGET), Debug)

CFLAGS += -DCELOGGER -g -ggdb 
CPPFLAGS += -DCELOGGER -g -ggdb

endif

ifeq ($(BUILD_TARGET), Release)

CFLAGS += $(OPTIMIZATIONS) -DCELOGGER
CPPFLAGS += $(OPTIMIZATIONS) -DCELOGGER

endif

############################### 
# Rules 
###############################

.cpp.o: 
	mkdir -p $(BUILD_TARGET)/$(@D) ; \
	$(ECHO) "Compiling" $< " --> " $(BUILD_TARGET)/$*.o " ..."; \
	$(CCPP) $(CPPFLAGS) -c $< -o $(BUILD_TARGET)/$@

.c.o:
	$(ECHO) "Compiling" $< " --> " $(BUILD_TARGET)/$*.o " ..."; \
	$(CC) $(CFLAGS) -c $< -o $(BUILD_TARGET)/$@

%.d : %.cpp
	$(ECHO) "Building dependency for " $<; \
	$(CCPP) $(CPPFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.cpp=.o)" "$<"
        
%.d : %.c
	$(ECHO) "Building dependency for " $<; \
	$(CCPP) $(CPPFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<"
        
#############################
# Targets
#############################

all: $(BUILD_TARGET)/$(EXEC_FILE)
default:   $(BUILD_TARGET)/$(EXEC_FILE)

ifneq  ($(MAKECMDGOALS),clean)
ifneq  ($(MAKECMDGOALS),copy)
include $(DEPEND_FILES)
endif
endif

$(BUILD_TARGET)/$(EXEC_FILE): $(OBJS)
	$(ECHO) "Linking " $@ "..."; \
	$(CD) ./$(BUILD_TARGET) ; \
	$(CCPP) $(LDFLAGS) $(LIBDIRS) -o ../$@ $(OBJS) $(LIBS);

copy:
	$(ECHO) "Copying lib"; \
	cp $(BUILD_TARGET)/$(EXEC_FILE) $(ANDROID_LIB_PATH)$(EXEC_FILE);

clean:
	$(ECHO) "Removing all .o files"; \
	find ./$(BUILD_TARGET) -name "*.o" -not -path "*/Lib/*" -print0 | xargs -0 rm -r; \
	$(ECHO) "Removing all .d files"; \
    find . -name "*.d" -not -path "*/Lib/*" -print0 | xargs -0 rm -r \
	
.SECONDEXPANSION:
$(BUILD_TARGET)/% : # directory creation rule
	mkdir -p $(BUILD_TARGET)/$@;