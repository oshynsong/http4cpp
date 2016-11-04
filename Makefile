##
## Main makefile for http4cpp
##
## author    Oshyn Song
## email     dualyangsong@gmail.com
##

DEBUG=0
INCLUDE_PATH=-I$(CURDIR)/src
LIB_PATH=-L/usr/lib
LIB=-lcurl -lpthread

CXXFLAGS=-Wall -pipe
SHARED_FLAGS=-fPIC -shared
ifeq ($(DEBUG), 1)
	CXXFLAGS+=-g -O0
else
	CXXFLAGS+=-O3
endif
CC=g++

OUT_PATH=$(CURDIR)/output
SRC_PATH= \
	$(CURDIR)/src \
	$(CURDIR)/src/common \
	$(CURDIR)/src/http
SOURCES:=$(foreach SRC,$(SRC_PATH),\
	$(wildcard $(SRC)/*.cpp)\
)
STATIC_OBJECTS:=$(foreach S,$(SOURCES),\
	$(addprefix $(OUT_PATH)/,\
		$(patsubst %.cpp,%.o,\
			$(subst $(CURDIR)/,,$(S))\
		)\
	)\
)
SHARED_OBJECTS:=$(foreach S,$(SOURCES),\
	$(addprefix $(OUT_PATH)/,\
		$(patsubst %.cpp,%.lib,\
			$(subst $(CURDIR)/,,$(S))\
		)\
	)\
)

STATIC=$(OUT_PATH)/libhttp4cpp.a
SHARED=$(OUT_PATH)/libhttp4cpp.so

# Starting here to construct
.PHONY: all
all: static shared

.PHONY: static
static: $(STATIC)
$(STATIC): \
		$(OUT_PATH)/src/common/util.o \
		$(OUT_PATH)/src/http/http_client.o \
		$(OUT_PATH)/src/http_request.o \
		$(OUT_PATH)/src/http_response.o
	@echo "Building static library $@..."
	ar -crv $@ $(STATIC_OBJECTS)
	@echo "Build $@ successfully!"

.PHONY: shared
shared: $(SHARED)
$(SHARED): \
		$(OUT_PATH)/src/common/util.lib \
		$(OUT_PATH)/src/http/http_client.lib \
		$(OUT_PATH)/src/http_request.lib \
		$(OUT_PATH)/src/http_response.lib

	@echo "Building shared library $@..."
	$(CC) $(LIB) $(LIB_PATH) -o $@ $^ $(CXXFLAGS) $(SHARED_FLAGS)
	@echo "Build $@ successfully!"

$(filter %.o,$(STATIC_OBJECTS)) : $(OUT_PATH)/%.o:$(CURDIR)/%.cpp
	@echo "Compiling $@ from $<..."
	@$(shell mkdir -p $(dir $@))
	$(CC) $(INCLUDE_PATH) $(CXXFLAGS) -c $< -o $@

$(filter %.lib,$(SHARED_OBJECTS)) : $(OUT_PATH)/%.lib:$(CURDIR)/%.cpp
	@echo "Compiling shared object $@ from $<..."
	@$(shell mkdir -p $(dir $@))
	$(CC) $(INCLUDE_PATH) $(CXXFLAGS) $(SHARED_FLAGS) -c $< -o $@

.PHONY: test
export CC LIB_PATH OUT_PATH LIB INCLUDE_PATH CXXFLAGS
test: $(CURDIR)/test
	@make -C $< DEBUG=$(DEBUG)

.PHONY : clean
clean:
	@$(RM) $(STATIC_OBJECTS) $(SHARED_OBJECTS) $(EXEC) $(SHARED)
	make -C $(CURDIR)/test clean
	@rm -rf $(OUT_PATH)/*
	@rm -rf $(OUT_PATH)

