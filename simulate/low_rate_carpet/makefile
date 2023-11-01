CC=g++
ROOT_DIR=$(shell pwd)
OBJS_DIR=objs
DEPS_DIR=deps
USELESS_LIBPCAP_LIB_DIR=../useless_libpcap_lib
USELESS_LIBPCAP_LIB_DIRS=$(patsubst $(USELESS_LIBPCAP_LIB_DIR),,$(shell find $(USELESS_LIBPCAP_LIB_DIR) -maxdepth 1 -type d))
USELESS_LIBPCAP_LIB_OBJS=$(foreach dir,$(USELESS_LIBPCAP_LIB_DIRS),$(shell if [ -f $(dir)/objs.txt ]; then cat $(dir)/objs.txt; fi))

LIBPCAPFLAGS = -lpcap
INCLUDE=$(addprefix -I ,$(USELESS_LIBPCAP_LIB_DIRS))

SRC=$(wildcard *.cpp)
OBJS = $(addprefix $(OBJS_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SRC))))
DEPS = $(addprefix $(DEPS_DIR)/,$(patsubst %.cpp,%.d,$(notdir $(SRC))))
TARGET=main

$(shell if [ ! -d $(OBJS_DIR) ]; then mkdir -p $(OBJS_DIR);fi)
$(shell if [ ! -d $(DEPS_DIR) ]; then mkdir -p $(DEPS_DIR);fi)

export CC ROOT_DIR OBJS_DIR DEPS_DIR

$(TARGET): $(USELESS_LIBPCAP_LIB_DIRS) $(OBJS) $(USELESS_LIBPCAP_LIB_OBJS)
	$(CC) -o $@ $(OBJS_DIR)/*.o $(INCLUDE) $(LIBPCAPFLAGS)
$(OBJS):$(OBJS_DIR)/%.o:%.cpp $(DEPS_DIR)/%.d
	$(CC) -c $< -o $@ $(INCLUDE)
$(DEPS):$(DEPS_DIR)/%.d:%.cpp
	touch $@
	echo -n $@ " " > $@
	$(CC) -MM $< >> $@ $(INCLUDE)
$(USELESS_LIBPCAP_LIB_DIRS): ECHO
	make -C $@
ECHO:
	@echo $(USELESS_LIBPCAP_LIB_DIRS)

.PHONY: clean
clean:
	-rm -r $(OBJS_DIR) $(DEPS_DIR) $(TARGET)

-include $(DEPS)

# OBJS_DIR=objs

# export ROOT_DIR
# export OBJS_DIR

# all:
# 	make -C useless_libpcap_lib/traffic_analyze/

# CC=g++
# ROOT_DIR=$(shell pwd)
# OBJ_DIR=objs
# DEP_DIR=deps

# $(shell if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR);fi)
# $(shell if [ ! -d $(DEP_DIR) ]; then mkdir -p $(DEP_DIR);fi)

# INCLUDE = -I include -I ../common
# LIBPCAPFLAGS = -lpcap

# SRC = $(wildcard src/*.cpp) $(wildcard ../common/*.cpp) $(wildcard src/traffic_generator/*.cpp) $(wildcard src/traffic_mixer/*.cpp) $(wildcard src/pcap_writer/*.cpp) $(wildcard src/strategy/*.cpp) $(wildcard src/pcap_reader/*.cpp)
# OBJS = $(addprefix $(OBJ_DIR),$(patsubst %.cpp,%.o,$(notdir $(SRC))))
# DEPS = $(addprefix $(DEP_DIR),$(patsubst %.cpp,%.d,$(notdir $(SRC))))
# TARGET = main

# vpath %.cpp src 
# vpath %.cpp src/traffic_generator
# vpath %.cpp src/traffic_mixer
# vpath %.cpp src/pcap_writer
# vpath %.cpp src/pcap_reader
# vpath %.cpp src/strategy
# vpath %.cpp ../common
# vpath %.h include
# vpath %.h ../common

# $(TARGET): $(OBJS)
# 	$(CC) -o $@ $(OBJS) $(INCLUDE) $(LIBPCAPFLAGS)
# $(OBJS):$(OBJ_DIR)%.o:%.cpp $(DEP_DIR)%.d
# 	$(CC) -c $< -o $@ $(INCLUDE)
# $(DEPS):$(DEP_DIR)%.d:%.cpp
# 	touch $@
# 	echo -n $@ " " > $@
# 	$(CC) -MM $< >> $@ $(INCLUDE)
# -include $(DEPS)

