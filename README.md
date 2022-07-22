## CREATING A INPUT FILE:

### required dependencies:
    sudo apt-get install libboost-all-dev
### to compile:
    g++ -o generate_input_file -O2 case-study/gen_input_file.cpp -l boost_program_options
### to run:
    ./generate_input_file --f="filename"

## RUNNING TESTS

### to compile:
    g++ -o tests -O2 case-study/tests.cpp lib/csvfile.cpp

### to run:
    ./tests