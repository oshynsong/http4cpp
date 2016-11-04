##
## Test makefile
##
## author    OshynSong
## email     dualyangsong@gmail.com
##

DEBUG=1

TEST_SOURCES=$(wildcard $(CURDIR)/*.cpp)
TEST_OBJECTS=$(addprefix $(OUT_PATH)/test/,\
	$(addsuffix .o,\
		$(basename $(notdir $(TEST_SOURCES))))\
)

util_test_exec=$(OUT_PATH)/test/util_test
http_test_exec=$(OUT_PATH)/test/http_test

EXEC=$(util_test_exec) \
	 $(http_test_exec)


.PHONY: all
all: $(EXEC)

$(util_test_exec): $(OUT_PATH)/test/util_test.o \
					$(OUT_PATH)/src/common/util.o
	@echo "Building $@ ..."
	$(CC) -o $@ $^ $(LIB_PATH) $(LIB)
	@echo "Building $@ successfully!"

$(http_test_exec): $(OUT_PATH)/test/http_test.o \
					$(OUT_PATH)/src/common/util.o \
					$(OUT_PATH)/src/http/http_client.o \
					$(OUT_PATH)/src/http_request.o \
					$(OUT_PATH)/src/http_response.o
	@echo "Building $@ ..."
	$(CC) -o $@ $^ $(LIB_PATH) $(LIB)
	@echo "Building $@ successfully!"

$(filter %.o,$(TEST_OBJECTS)) : $(OUT_PATH)/test/%.o:$(CURDIR)/%.cpp
	@echo "Compiling $@ ..."
	@$(shell mkdir -p $(dir $@))
	$(CC) $(INCLUDE_PATH) $(CXXFLAGS) -c $< -o $@


.PHONY : clean
clean:
	@$(RM) $(TEST_OBJECTS) $(EXEC)
	@rm -rf $(OUT_PATH)/test

