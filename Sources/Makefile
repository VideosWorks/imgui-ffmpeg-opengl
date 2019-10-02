# Makefile pour le projet miniDart
# Copyright : Eric Bachard  / dimanche 4 septembre 2016, 21:24:45 (UTC+0200)

# This makefile is under GPL v2 license
# see : http://www.gnu.org/licenses/gpl-2.0.html


MY_LOCALE = french

#GCC_VERSION = -7
#G++_VERSION = -7

BUILD_DIR = build
SOURCES_DIR = src
OBJ_DIR = obj

VERSION_MAJOR = 0
VERSION_MINOR = 9
#VERSION_MICRO = 020_embedded_player_decoupled_webcam_and_new_engine09
VERSION_MICRO = 6.2
VERSION = ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}

#APPLICATION = miniDart_${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_MICRO}
APPLICATION = miniDart

UNAME_S = $(shell uname -s)

ifneq (${TARGET}, WINDOWS)
	OS_FLAGS += -DBUILDING_LINUX_VERSION
endif

ifeq (${MY_LOCALE}, french)
        OS_FLAGS += -DBUILDING_FRENCH
else
        OS_FLAGS += -DBUILDING_EN_US_VERSION
endif

# available for both Windows and Linux
#EXTRA_OS_FLAGS = -DNOT_RELEASE_VERSION
#EXTRA_OS_FLAGS = -DFPS_TEST
#EXTRA_OS_FLAGS += -DTESTING_COLORS

#CC_OPT_LEVEL = -O3
#CXX_OPT_LEVEL = -O3
CC_OPT_LEVEL = -O3 -march=native -mtune=native
CXX_OPT_LEVEL = -O3 -march=native -mtune=native
#DEBUG = -g

CXX_STD = -std=c++11

##################################
# LINUX HOST BUILD CONFIGURATION #
##################################

##!###CC_OPT_LEVEL  += -march=native -mtune=native
##!###CXX_OPT_LEVEL += -march=native -mtune=native

FFMPEG_FLAGS = $(shell pkg-config --cflags --libs libavutil libavformat libavcodec libavfilter libswscale libswresample)
#FFMPEG_FLAGS = $(shell pkg-config --cflags --libs libavutil libavdevice libavformat libavcodec libavfilter libswscale libswresample)

LINUX_FLAGS = -D__Linux 
#EXTRA_OS_FLAGS += -DNOT_RELEASE_VERSION
EXTRA_OS_FLAGS += -DFPS_FEATURE_ENABLED
OS_FLAGS += ${EXTRA_OS_FLAGS} -D__Linux -DNATIVE_BUILD
UNAME_S = $(shell uname -s)

#ifeq ( $(UNAME_S), Linux)
	OS_FLAGS += $(LINUX_FLAGS)
#endif

CC_OPT_LEVEL = -O3 -march=native -mtune=native
CXX_OPT_LEVEL = -O3 -march=native -mtune=native
#DEBUG = -g

CC = gcc${GCC_VERSION} $(CC_OPT_LEVEL) $(DEBUG)
#####CXX = /usr/bin/g++ -O3
#CXX = /usr/bin/g++
CXX = g++${G++_VERSION} $(CXX_OPT_LEVEL) $(DEBUG)
# tested : std=c++14  <= works already !
CXX_STD = -std=c++11
#CC_FLAGS = -Wall -ansi -pedantic -fno-builtin
####CC_FLAGS = -Wall -pedantic -fno-builtin
CC_FLAGS = -Wall -fPIC
#CC_FLAGS += -DUSE_GL3
CXX_FLAGS = ${CXX_STD} ${CC_FLAGS}
CC_STD = -std=c99

OPENCV_FLAGS = `pkg-config opencv --cflags --libs`
OPENCV_FLAGS_STATIC = `pkg-config --cflags --libs opencv_static`


APPLICATION_NAME = ${APPLICATION}${TARGET_EXTENSION}

EXTRA_FLAGS =
#EXTRA_FLAGS = -DBLUE_STYLE
EXTRA_FLAGS += -DHIGH_DEFINITION_CAPTURE -DIMGUI_IMPL_OPENGL_LOADER_GL3W

BINARY_NAME = ${BUILD_DIR}/${APPLICATION_NAME}

OUTBIN = ${BUILD_DIR}/${APPLICATION_NAME}
static = ${BUILD_DIR}/${APPLICATION_NAME}_s
OUTBIN_DEBUG = ${BUILD_DIR}/${APPLICATION_NAME}_debug
OUTBIN_ALL = 	$(OUTBIN) 	$(OUTBIN_DEBUG)

INCLUDE_BASE = -I./inc -I./localization
3RD_PARTY_BASE = ./src/3rdparty
INCLUDE_3RD_PARTY_BASE = -I$(3RD_PARTY_BASE)
INCLUDE_ICON_FONTS_BASE = -I./fonts

IMGUI_INC = $(INCLUDE_3RD_PARTY_BASE)/imgui
GL3W_INC = $(INCLUDE_3RD_PARTY_BASE)/imgui/gl3w

TINYXML2_INC = $(INCLUDE_3RD_PARTY_BASE)/tinyxml2/src

STB_INC = $(INCLUDE_3RD_PARTY_BASE)/stb
NFD_INC = $(INCLUDE_3RD_PARTY_BASE)/nativefiledialogs

#YUV2RGB_INC = $(INCLUDE_3RD_PARTY_BASE)/yuv2rgb


FONT_AWESOME_INC = $(INCLUDE_ICON_FONTS_BASE)/Icons/FontAwesome

MATERIAL_DESIGN_INC = $(INCLUDE_ICON_FONTS_BASE)/Icons/MaterialDesign

### TESTING PURPOSE
###LIBOPENSHOT_INC = $(INCLUDE_3RD_PARTY_BASE)/libopenshot/include

VIDEO_SEEKER_INC = $(INCLUDE_3RD_PARTY_BASE)/video_seeker

DELAY_INC = $(INCLUDE_3RD_PARTY_BASE)/delay/inc

#GFX_UI_INC = $(INCLUDE_3RD_PARTY_BASE)/gfx_ui

#SDL2_FFMPEG_INC = $(INCLUDE_3RD_PARTY_BASE)/sdl2_ffmpeg/include

INCLUDE_DIR +=  $(INCLUDE_BASE) \
                $(IMGUI_INC) \
                $(GL3W_INC) \
                $(STB_INC) \
                $(NFD_INC) \
                $(TINYXML2_INC) \
                $(FONT_AWESOME_INC) \
                $(MATERIAL_DESIGN_INC) \
                $(YUV2RGB_INC) \
                $(SDL2_FFMPEG_INC)

#                $(LIBOPENSHOT_INC)
#                $(GFX_UI_INC)

FILES = *.cpp

IMGUI_FILES = $(shell ls $(3RD_PARTY_BASE)/imgui/*.cpp)
GL3W_FILES = $(shell ls $(3RD_PARTY_BASE)/imgui/gl3w/*.c)
TINYXML2_FILES = $(shell ls $(3RD_PARTY_BASE)/tinyxml2/src/*.cpp)

FILES += $(IMGUI_FILES) $(GL3W_FILES) $(TINYXML2_FILES)

# C_FILES
NFD_C_FILES = $(shell ls $(3RD_PARTY_BASE)/nativefiledialogs/*.c)

DEPS = $^


##################################
# LINUX HOST BUILD CONFIGURATION #
##################################

# commented : not available
# uncomment : add opencv crop (broken)

SDL2_FLAGS += $(shell sdl2-config --cflags --libs)
#SDL2_FLAGS +=
SDL2_FLAGS_STATIC = -lSDL2_image-2.0 $(shell sdl2-config --cflags --static-libs)

GTK+-3.0_FLAGS += $(shell pkg-config --cflags --libs gtk+-3.0)
GTK+-2.0_FLAGS += $(shell pkg-config --cflags --libs gtk+-2.0)
GTK+-2.0_FLAGS_STATIC =  -lgobject-2.0 -lglib-2.0 -lgtk-x11-2.0  $(shell PKG_CONFIG_PATH=/usr/lib/${WIN_ARCH}-linux-gnu/pkgconfig pkg-config --cflags --libs gtk+-2.0) -lpng12

ifdef CROP
	GTK+FLAGS += ${GTK+-2.0_FLAGS}
	EXTRA_FLAGS += -DCROP_IMAGE_AVAILABLE
else
	GTK+FLAGS += ${GTK+-3.0_FLAGS}
endif

#GTK3_LIBS = $(shell pkg-config --cflags --libs gtk+-3.0)
#SDL2_LIBS = $(shell sdl2-config --cflags --libs)
#GLIB_LIBS= $(shell pkg-config --libs glib-2.0)

OPENGL_FLAGS = -lGL

LD_FLAGS = -lm -lass ${OPENGL_FLAGS} -ldl ${OPENCV_FLAGS} ${GTK+FLAGS} ${FFMPEG_FLAGS} ${GLFW3_FLAGS} ${SDL2_FLAGS}
LD_FLAGS_STATIC = -lm -lass ${OPENGL_FLAGS} -ldl -lz -lpng12 ${GTK+-2.0_FLAGS_STATIC} ${OPENCV_FLAGS_STATIC} ${FFMPEG_FLAGS} ${SDL2_FLAGS_STATIC}  -static-libstdc++

GCC_SECURITY_FLAGS = -fstack-protector -pie -fPIE -D_FORTIFY_SOURCE=2 -Wformat -Wformat-security

CFLAGS = ${CC_FLAGS} ${CC_STD} ${OS_FLAGS} ${EXTRA_FLAGS}
CFLAGS_DEBUG = -g -DDEBUG -DUNIT_TESTS=${QA_TESTS}

C++FLAGS = ${CXX_FLAGS} ${CXX_STD} ${OS_FLAGS} ${EXTRA_FLAGS}
C++FLAGS_DEBUG = -g -DDEBUG

OBJS = ${SOURCES_DIR}/${FILES}

ARCHIVE_EXT = .tar.gz

TO_BE_ZIPPED = \
	sound \
	miniDart_features.txt \
	documentation \
	inc \
	src \
	fonts \
	images \
	localization \
	resources \
	Makefile \
	Changelog \
	ROADMAP \
	TODO

#suffix :
#	*.c:$(OBJ_DIR)/*.o

TO_BE_ZIPPED_BINARIES = \
	${BUILD_DIR}

# here starts binaries creation

ifdef TARGET
all : clean $(OUTBIN)
else
#all : clean $(OUTBIN) $(static)
all : clean $(OUTBIN)
#all : clean $(OUTBIN)
endif

#SRC= $(wildcard *.c)
#OBJ= $(SRC:.c=.o)

#$(OBJ:.o=obj\/.o)
#$(patsubst %.o, %obj/.o, $(C_FILES) $(C++_FILES) )
#$(addprefix obj/, *.o)

$(static) : C_FILES_S C++_FILES_S
	$(CXX) $(C++FLAGS) -DSTATIC_BUILD ${GCC_SECURITY_FLAGS} $(INCLUDE_DIR) *.o -o $@ $(LD_FLAGS_STATIC)
	@echo "cleanup ..."
	@echo "$(static) build completed"
	$(RM) -f *.o
	strip $(static)
	@echo "done"



$(OUTBIN) : C_FILES C++_FILES
	$(CXX) $(C++FLAGS) ${GCC_SECURITY_FLAGS} $(INCLUDE_DIR) *.o -o $@ $(LD_FLAGS)
	$(RM) -f *.o
	@echo "cleanup ..."
	@echo "$(OUTBIN) build completed"
	strip $(OUTBIN)
	@echo "done"



#C++_FILES : src/*.cpp src/3rdparty/nativefiledialogs/*.cpp src/3rdparty/imgui/*.cpp $(GL3W_FILES) $(TINYXML2_FILES) $(GFX_UI_FILES)
#C++_FILES : src/*.cpp src/3rdparty/nativefiledialogs/*.cpp src/3rdparty/imgui/*.cpp $(GL3W_FILES)
C++_FILES : src/*.cpp src/3rdparty/nativefiledialogs/*.cpp $(IMGUI_FILES) $(GL3W_FILES) $(VIDEO_SEEKER_FILES)
	$(CXX) $(INCLUDE_DIR) $(C++FLAGS) -c $(LD_FLAGS) $?

C++_FILES_S : src/*.cpp src/3rdparty/nativefiledialogs/*.cpp $(IMGUI_FILES) $(GL3W_FILES) $(VIDEO_SEEKER_FILES)
	$(CXX) -DSTATIC_BUILD $(INCLUDE_DIR) $(C++FLAGS) -c $(LD_FLAGS_STATIC) $?

# src/3rdparty/yuv2rgb/*.c

C_FILES : src/3rdparty/nativefiledialogs/*.c  $(SDL2_FFMPEG_FILES) $(DELAY_FILES)
	$(CC) $(CFLAGS) -c $(INCLUDE_DIR) ${GTK+FLAGS} $(CFLAGS) $? $(SDL2_FLAGS)

# src/3rdparty/yuv2rgb/*.c

C_FILES_S : src/3rdparty/nativefiledialogs/*.c  $(SDL2_FFMPEG_FILES) $(DELAY_FILES)
	$(CC) -DSTATIC_BUILD $(CFLAGS) -c $(INCLUDE_DIR) ${GTK+-2.0_FLAGS_STATIC}  $(CFLAGS) $? $(SDL2_FLAGS)



# FIXME : implement debug build
#$(OUTBIN_DEBUG) : $(OBJS)
#	$(CXX) $(C++FLAGS) ${GCC_SECURITY_FLAGS} $(C++FLAGS_DEBUG) $(INCLUDE_DIR) -o $(OUTBIN_DEBUG) $(DEPS) $(LD_FLAGS)
#
#	$(CXX) $(C++FLAGS) ${GCC_SECURITY_FLAGS} $(C++FLAGS_DEBUG) $(INCLUDE_DIR)  -o $(OUTBIN_DEBUG) $(DEPS) $(SOURCES_DIR)/sdl_gpu/common.c $(SOURCES_DIR)/sdl_gpu/demo-font.c $(C_FILES)  $(LD_FLAGS)

#  full line, including removing Windows build when building Linux version et vice versa
#	rm -rf $(OUTBIN) $(OUTBIN_DEBUG) $(BUILD_DIR)/${APPLICATION}  $(OUTBIN).exe $(OUTBIN_DEBUG).exe ${EXEC_NAME}_${VERSION}

####$(OUTBIN_DEBUG) $(OUTBIN_DEBUG).exe

clean :
	rm -rf *.o $(OUTBIN) ${BUILD_DIR}/${APPLICATION}_s${TARGET_EXTENSION} ${BUILD_DIR}/${APPLICATION}_${VERSION}${TARGET_EXTENSION}
	@echo Fichiers binaires supprimés.

devel: clean
	mkdir ${APPLICATION}_${VERSION}
	mkdir -p ${APPLICATION}_${VERSION}/build
	cp -R ${TO_BE_ZIPPED} ${APPLICATION}_${VERSION}
	tar cvzf ${APPLICATION}_${VERSION}${ARCHIVE_EXT} ${APPLICATION}_${VERSION}
	rm -rf ${APPLICATION}_${VERSION}


archive : all
	mkdir ${APPLICATION}_${VERSION}
	cp -R ${TO_BE_ZIPPED} ${APPLICATION}_${VERSION}
	cp -R ${TO_BE_ZIPPED_BINARIES} ${APPLICATION}_${VERSION}
	tar cvzf ${APPLICATION}_${VERSION}${ARCHIVE_EXT} ${APPLICATION}_${VERSION}

real_clean :
	rm -rf *.o $(OUTBIN) ${BUILD_DIR}/${APPLICATION}_s${TARGET_EXTENSION} ${BUILD_DIR}/${APPLICATION}_${VERSION}${TARGET_EXTENSION}
ifneq (${TARGET_EXTENSION}, .exe)
	rm -rf *.o ${BUILD_DIR}/${APPLICATION}.exe ${BUILD_DIR}/${APPLICATION}_s.exe ${BUILD_DIR}/${APPLICATION}_${VERSION}${TARGET_EXTENSION}.exe
else
	rm -rf *.o ${BUILD_DIR}/${APPLICATION} ${BUILD_DIR}/${APPLICATION}_s ${BUILD_DIR}/${APPLICATION}_${VERSION}
endif
	rm -rf ${APPLICATION}_*${ARCHIVE_EXT}

# FIXME : missing Windows packaging
