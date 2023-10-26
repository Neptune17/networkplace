CC=g++
OBJ_DIR=objs/
DEP_DIR=deps/

$(shell if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR);fi)
$(shell if [ ! -d $(DEP_DIR) ]; then mkdir -p $(DEP_DIR);fi)

INCLUDE = -I include
LIBPCAPFLAGS = -lpcap

SRC = $(wildcard src/*.cpp) $(wildcard src/feature_extractor/*.cpp) $(wildcard src/pcap_reader/*.cpp) $(wildcard src/utils/*.cpp)
OBJS = $(addprefix $(OBJ_DIR),$(patsubst %.cpp,%.o,$(notdir $(SRC))))
DEPS = $(addprefix $(DEP_DIR),$(patsubst %.cpp,%.d,$(notdir $(SRC))))
TARGET = main

vpath %.cpp src 
vpath %.cpp src/feature_extractor 
vpath %.cpp src/pcap_reader
vpath %.cpp src/utils
vpath %.h include

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(INCLUDE) $(LIBPCAPFLAGS)
$(OBJS):$(OBJ_DIR)%.o:%.cpp $(DEP_DIR)%.d
	$(CC) -c $< -o $@ $(INCLUDE)
$(DEPS):$(DEP_DIR)%.d:%.cpp
	touch $@
	echo -n $@ " " > $@
	$(CC) -MM $< >> $@ $(INCLUDE)
-include $(DEPS)

.PHONY: clean
clean:
	-rm -r $(OBJ_DIR) $(DEP_DIR) $(TARGET)