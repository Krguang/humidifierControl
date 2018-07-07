#Generated by VisualGDB project wizard. 
#Note: VisualGDB will automatically update this file when you add new sources to the project.
#All other changes you make in this file will be preserved.
#Visit http://visualgdb.com/makefiles for more details

#VisualGDB: AutoSourceFiles		#<--- remove this line to disable auto-updating of SOURCEFILES and EXTERNAL_LIBS

TARGETNAME := humidifierControl.elf
#TARGETTYPE can be APP, STATIC or SHARED
TARGETTYPE := APP

to_lowercase = $(subst A,a,$(subst B,b,$(subst C,c,$(subst D,d,$(subst E,e,$(subst F,f,$(subst G,g,$(subst H,h,$(subst I,i,$(subst J,j,$(subst K,k,$(subst L,l,$(subst M,m,$(subst N,n,$(subst O,o,$(subst P,p,$(subst Q,q,$(subst R,r,$(subst S,s,$(subst T,t,$(subst U,u,$(subst V,v,$(subst W,w,$(subst X,x,$(subst Y,y,$(subst Z,z,$1))))))))))))))))))))))))))

CONFIG ?= DEBUG

CONFIGURATION_FLAGS_FILE := $(call to_lowercase,$(CONFIG)).mak
TOOLCHAIN_ROOT ?= $(toolchain_root)

include $(CONFIGURATION_FLAGS_FILE)

#LINKER_SCRIPT defined inside the configuration file (e.g. debug.mak) should override any linker scripts defined in shared .mak files
CONFIGURATION_LINKER_SCRIPT := $(LINKER_SCRIPT)

include $(ADDITIONAL_MAKE_FILES)

ifneq ($(CONFIGURATION_LINKER_SCRIPT),)
LINKER_SCRIPT := $(CONFIGURATION_LINKER_SCRIPT)
endif

ifneq ($(LINKER_SCRIPT),)
LDFLAGS += -T$(LINKER_SCRIPT)
endif

ifeq ($(BINARYDIR),)
error:
	$(error Invalid configuration, please check your inputs)
endif

SOURCEFILES := Src/adc.c Src/dataProcessing.c Src/dma.c Src/freertos.c Src/gpio.c Src/humiCtrl.c Src/main.c Src/modbusMaster.c Src/modbusSlave.c Src/stm32f1xx_hal_msp.c Src/stm32f1xx_hal_timebase_TIM.c Src/stm32f1xx_it.c Src/system_stm32f1xx.c Src/tim.c Src/usart.c $(BSP_ROOT)/STM32F1xxxx/StartupFiles/startup_stm32f103xb.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc_ex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_can.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cec.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_crc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac_ex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_eth.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_hcd.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2s.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_irda.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_mmc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nand.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nor.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pccard.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sd.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_smartcard.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi_ex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sram.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_wwdg.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_adc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_crc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_dac.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_dma.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_exti.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_fsmc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_gpio.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_i2c.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_pwr.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_rcc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_rtc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_sdmmc.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_spi.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_tim.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usart.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usb.c $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_utils.c $(BSP_ROOT)/FreeRTOS/Source/croutine.c $(BSP_ROOT)/FreeRTOS/Source/event_groups.c $(BSP_ROOT)/FreeRTOS/Source/list.c $(BSP_ROOT)/FreeRTOS/Source/queue.c $(BSP_ROOT)/FreeRTOS/Source/tasks.c $(BSP_ROOT)/FreeRTOS/Source/timers.c $(BSP_ROOT)/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c $(BSP_ROOT)/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c $(BSP_ROOT)/FreeRTOS/Source/portable/MemMang/heap_4.c
EXTERNAL_LIBS := 
EXTERNAL_LIBS_COPIED := $(foreach lib, $(EXTERNAL_LIBS),$(BINARYDIR)/$(notdir $(lib)))

CFLAGS += $(COMMONFLAGS)
CXXFLAGS += $(COMMONFLAGS)
ASFLAGS += $(COMMONFLAGS)
LDFLAGS += $(COMMONFLAGS)

CFLAGS += $(addprefix -I,$(INCLUDE_DIRS))
CXXFLAGS += $(addprefix -I,$(INCLUDE_DIRS))

CFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))
CXXFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))
ASFLAGS += $(addprefix -D,$(PREPROCESSOR_MACROS))

CXXFLAGS += $(addprefix -framework ,$(MACOS_FRAMEWORKS))
CFLAGS += $(addprefix -framework ,$(MACOS_FRAMEWORKS))
LDFLAGS += $(addprefix -framework ,$(MACOS_FRAMEWORKS))

LDFLAGS += $(addprefix -L,$(LIBRARY_DIRS))

ifeq ($(GENERATE_MAP_FILE),1)
LDFLAGS += -Wl,-Map=$(BINARYDIR)/$(basename $(TARGETNAME)).map
endif

LIBRARY_LDFLAGS = $(addprefix -l,$(LIBRARY_NAMES))

ifeq ($(IS_LINUX_PROJECT),1)
	RPATH_PREFIX := -Wl,--rpath='$$ORIGIN/../
	LIBRARY_LDFLAGS += $(EXTERNAL_LIBS)
	LIBRARY_LDFLAGS += -Wl,--rpath='$$ORIGIN'
	LIBRARY_LDFLAGS += $(addsuffix ',$(addprefix $(RPATH_PREFIX),$(dir $(EXTERNAL_LIBS))))
	
	ifeq ($(TARGETTYPE),SHARED)
		CFLAGS += -fPIC
		CXXFLAGS += -fPIC
		ASFLAGS += -fPIC
		LIBRARY_LDFLAGS += -Wl,-soname,$(TARGETNAME)
	endif
	
	ifneq ($(LINUX_PACKAGES),)
		PACKAGE_CFLAGS := $(foreach pkg,$(LINUX_PACKAGES),$(shell pkg-config --cflags $(pkg)))
		PACKAGE_LDFLAGS := $(foreach pkg,$(LINUX_PACKAGES),$(shell pkg-config --libs $(pkg)))
		CFLAGS += $(PACKAGE_CFLAGS)
		CXXFLAGS += $(PACKAGE_CFLAGS)
		LIBRARY_LDFLAGS += $(PACKAGE_LDFLAGS)
	endif	
else
	LIBRARY_LDFLAGS += $(EXTERNAL_LIBS)
endif

LIBRARY_LDFLAGS += $(ADDITIONAL_LINKER_INPUTS)

all_make_files := $(firstword $(MAKEFILE_LIST)) $(CONFIGURATION_FLAGS_FILE) $(ADDITIONAL_MAKE_FILES)

ifeq ($(STARTUPFILES),)
	all_source_files := $(SOURCEFILES)
else
	all_source_files := $(STARTUPFILES) $(filter-out $(STARTUPFILES),$(SOURCEFILES))
endif

source_obj1 := $(all_source_files:.cpp=.o)
source_obj2 := $(source_obj1:.c=.o)
source_obj3 := $(source_obj2:.s=.o)
source_obj4 := $(source_obj3:.S=.o)
source_obj5 := $(source_obj4:.cc=.o)
source_objs := $(source_obj5:.cxx=.o)

all_objs := $(addprefix $(BINARYDIR)/, $(notdir $(source_objs)))

PRIMARY_OUTPUTS :=

ifeq ($(GENERATE_BIN_FILE),1)
PRIMARY_OUTPUTS += $(BINARYDIR)/$(basename $(TARGETNAME)).bin
endif

ifeq ($(GENERATE_IHEX_FILE),1)
PRIMARY_OUTPUTS += $(BINARYDIR)/$(basename $(TARGETNAME)).ihex
endif

ifeq ($(PRIMARY_OUTPUTS),)
PRIMARY_OUTPUTS := $(BINARYDIR)/$(TARGETNAME)
endif

all: $(PRIMARY_OUTPUTS)

$(BINARYDIR)/$(basename $(TARGETNAME)).bin: $(BINARYDIR)/$(TARGETNAME)
	$(OBJCOPY) -O binary $< $@

$(BINARYDIR)/$(basename $(TARGETNAME)).ihex: $(BINARYDIR)/$(TARGETNAME)
	$(OBJCOPY) -O ihex $< $@
	
ifneq ($(LINKER_SCRIPT),)
$(BINARYDIR)/$(TARGETNAME): $(LINKER_SCRIPT)
endif

ifeq ($(TARGETTYPE),APP)
$(BINARYDIR)/$(TARGETNAME): $(all_objs) $(EXTERNAL_LIBS)
	$(LD) -o $@ $(LDFLAGS) $(START_GROUP) $(all_objs) $(LIBRARY_LDFLAGS) $(END_GROUP)
endif

ifeq ($(TARGETTYPE),SHARED)
$(BINARYDIR)/$(TARGETNAME): $(all_objs) $(EXTERNAL_LIBS)
	$(LD) -shared -o $@ $(LDFLAGS) $(START_GROUP) $(all_objs) $(LIBRARY_LDFLAGS) $(END_GROUP)
endif
	
ifeq ($(TARGETTYPE),STATIC)
$(BINARYDIR)/$(TARGETNAME): $(all_objs)
	$(AR) -r $@ $^
endif

-include $(all_objs:.o=.dep)

clean:
ifeq ($(USE_DEL_TO_CLEAN),1)
	cmd /C del /S /Q $(BINARYDIR)
else
	rm -rf $(BINARYDIR)
endif

$(BINARYDIR):
	mkdir $(BINARYDIR)

#VisualGDB: FileSpecificTemplates		#<--- VisualGDB will use the following lines to define rules for source files in subdirectories
$(BINARYDIR)/%.o : %.cpp $(all_make_files) |$(BINARYDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.S $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(ASFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.s $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(ASFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.cc $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

$(BINARYDIR)/%.o : %.cxx $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) $(CXXFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

#VisualGDB: GeneratedRules				#<--- All lines below are auto-generated. Remove this line to suppress auto-generation of file rules.


$(BINARYDIR)/adc.o : Src/adc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/dataProcessing.o : Src/dataProcessing.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/dma.o : Src/dma.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/freertos.o : Src/freertos.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/gpio.o : Src/gpio.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/humiCtrl.o : Src/humiCtrl.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/main.o : Src/main.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/modbusMaster.o : Src/modbusMaster.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/modbusSlave.o : Src/modbusSlave.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_msp.o : Src/stm32f1xx_hal_msp.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_timebase_TIM.o : Src/stm32f1xx_hal_timebase_TIM.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_it.o : Src/stm32f1xx_it.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/system_stm32f1xx.o : Src/system_stm32f1xx.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/tim.o : Src/tim.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/usart.o : Src/usart.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/startup_stm32f103xb.o : $(BSP_ROOT)/STM32F1xxxx/StartupFiles/startup_stm32f103xb.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_adc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_adc_ex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_adc_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_can.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_can.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_cec.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cec.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_cortex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_crc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_crc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_dac.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_dac_ex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dac_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_dma.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_eth.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_eth.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_flash.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_flash_ex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_gpio.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_gpio_ex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_hcd.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_hcd.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_i2c.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_i2s.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2s.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_irda.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_irda.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_iwdg.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_mmc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_mmc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_nand.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nand.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_nor.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_nor.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_pccard.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pccard.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_pcd.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_pcd_ex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_pwr.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_rcc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_rcc_ex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_rtc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_rtc_ex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_sd.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sd.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_smartcard.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_smartcard.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_spi.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_spi_ex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_sram.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sram.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_tim.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_tim_ex.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_uart.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_usart.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_hal_wwdg.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_wwdg.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_adc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_adc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_crc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_crc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_dac.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_dac.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_dma.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_dma.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_exti.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_exti.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_fsmc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_fsmc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_gpio.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_gpio.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_i2c.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_i2c.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_pwr.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_pwr.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_rcc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_rcc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_rtc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_rtc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_sdmmc.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_sdmmc.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_spi.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_spi.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_tim.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_tim.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_usart.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usart.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_usb.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usb.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/stm32f1xx_ll_utils.o : $(BSP_ROOT)/STM32F1xxxx/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_utils.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/croutine.o : $(BSP_ROOT)/FreeRTOS/Source/croutine.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/event_groups.o : $(BSP_ROOT)/FreeRTOS/Source/event_groups.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/list.o : $(BSP_ROOT)/FreeRTOS/Source/list.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/queue.o : $(BSP_ROOT)/FreeRTOS/Source/queue.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/tasks.o : $(BSP_ROOT)/FreeRTOS/Source/tasks.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/timers.o : $(BSP_ROOT)/FreeRTOS/Source/timers.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/cmsis_os.o : $(BSP_ROOT)/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/port.o : $(BSP_ROOT)/FreeRTOS/Source/portable/GCC/ARM_CM3/port.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)


$(BINARYDIR)/heap_4.o : $(BSP_ROOT)/FreeRTOS/Source/portable/MemMang/heap_4.c $(all_make_files) |$(BINARYDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MD -MF $(@:.o=.dep)

