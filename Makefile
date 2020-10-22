TARGET		:= GTA3
TITLE		:= GTA000003

SOURCES		:= src src/animation src/audio src/audio/oal src/audio/eax src/control src/core src/entities src/math src/modelinfo src/objects src/peds src/render src/rw src/save src/skel src/skel/glfw src/text src/vehicles src/weapons src/extras src/fakerw
INCLUDES	:= src src/animation src/audio src/audio/oal src/audio/eax src/control src/core src/entities src/math src/modelinfo src/objects src/peds src/render src/rw src/save src/skel src/skel/glfw src/text src/vehicles src/weapons src/extras src/fakerw librw

CFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.c))
CPPFILES   := $(foreach dir,$(SOURCES), $(wildcard $(dir)/*.cpp))
BINFILES := $(foreach dir,$(DATA), $(wildcard $(dir)/*.bin))
OBJS     := $(addsuffix .o,$(BINFILES)) $(CFILES:.c=.o) $(CPPFILES:.cpp=.o) 

INCLUDE	:=	$(foreach dir,$(INCLUDES),-I$(dir))

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CXX      = $(PREFIX)-g++
ARCH	:=	-mtune=cortex-a9 -march=armv7-a -mfpu=neon
CFLAGS	:=	-g -Wl,-q -fno-optimize-sibling-calls -O2 -ftree-vectorize -fsingle-precision-constant -mfloat-abi=hard -ffast-math $(ARCH) $(DEFINES)
CFLAGS	+=	$(INCLUDE) -DPSP2 -DMASTER -DFINAL -DLIBRW -DRW_GL3 -DAUDIO_OAL -DLIBRW_GLAD
CXXFLAGS	:= $(CFLAGS) -fno-rtti -fno-exceptions
ASFLAGS	:=	-g $(ARCH)
LDFLAGS	=	-g $(ARCH) -Wl,-Map,$(notdir $*.map)
LIBS	:=  -lrw -lopenal -lSDL2 -lvita2d -lvitagl -lSceAppMgr_stub -lSceDisplay_stub -lSceCommonDialog_stub -lSceLibKernel_stub \
			-lSceSysmodule_stub -lvitashark -lSceShaccCg_stub -lvitagl -lmathneon -lSceGxm_stub -lScePower_stub \
			-lSceCtrl_stub -lSceHid_stub -lSceAudio_stub -lSceTouch_stub -lm -lpthread -lmpg123

all: $(TARGET).vpk

$(TARGET).vpk: $(TARGET).velf
	vita-make-fself -c -s $< build/eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE) -d ATTRIBUTE2=12 "Grand Theft Auto III" param.sfo
	cp -f param.sfo build/sce_sys/param.sfo

	#------------ Comment this if you don't have 7zip ------------------
	7z a -tzip ./$(TARGET).vpk -r ./build/sce_sys ./build/eboot.bin
	#-------------------------------------------------------------------

%.velf: %.elf
	cp $< $<.unstripped.elf
	$(PREFIX)-strip -g $<
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean:
	@rm -rf $(TARGET).velf $(TARGET).elf $(OBJS) $(TARGET).elf.unstripped.elf $(TARGET).vpk build/eboot.bin build/sce_sys/param.sfo ./param.sfo