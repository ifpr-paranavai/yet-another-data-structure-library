GPP = g++
BIN_NAME = tests
DEBUG_BIN_NAME = tests_dbg
O2 = -O2
BOOST_PARAM = -l boost_program_options
SOURCE_CODES = tests.cpp ../lib/csvfile.cpp ../lib.cpp

tests:
	@echo compiling the tests -- binary name = $(BIN_NAME) 
	$(GPP) -o $(BIN_NAME) $(O2) $(SOURCE_CODES) $(BOOST_PARAM) -lm

clean:
	rm -f $(BIN_NAME)

debug:
	@echo compiling on debug mode -- binary name = $(BIN_NAME)
	$(GPP) -o $(DEBUG_BIN_NAME) $(SOURCE_CODES) $(BOOST_PARAM) -lm -g 

dclean:
	rm -f $(DEBUG_BIN_NAME)

run:
	@./$(BIN_NAME)

run-debug:
	gdb ./$(DEBUG_BIN_NAME)

help-debug:
	@echo first of all, compile the binary with "-g"
	@echo enter "gdb" console by typing gdb
	@echo use "file" passing the bin name
	@echo use "run" and pass in any arguments the application needs to start
	@echo when the Segmentation Fault happens, type "bt" to get its stack trace