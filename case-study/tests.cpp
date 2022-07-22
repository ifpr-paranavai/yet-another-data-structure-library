#include <filesystem>
#include <iostream>
#include <string>

#include "../lib/csvfile.h"
#include "../lib/timer.h"

#include "../vector.h"
#include "../list.h"

#define ELEMENTS_AMOUNT (2000000)
#define TESTS_AMOUNT (30)

enum class operation_t
{
    insertion,
    search,
    deletion,
}; 

static int buffer[ELEMENTS_AMOUNT];
std::filesystem::path current_path = std::filesystem::current_path();
std::string input_files_path = current_path.string()+"/case-study/input-files/";
std::string datasets_path = current_path.string()+"/case-study/datasets/";

static void read_input_file_into_buffer (const char *filename)
{
    FILE *fp = fopen((input_files_path+filename).c_str(), "rb");
    fread((void *)buffer, sizeof(int), ELEMENTS_AMOUNT, fp);
    fclose(fp);
}

static void vector_operation_test (const operation_t& operation, double *times_buffer)
{
    lib::timer_t timer;

    for (int i = 0; i < TESTS_AMOUNT; i++) {
        std::string input_filename("inputs" + std::to_string(i + 1) + ".bin");
        read_input_file_into_buffer(input_filename.c_str());

        yadsl::vector_t<int> vector(ELEMENTS_AMOUNT);

        if (operation == operation_t::search || operation == operation_t::deletion)
            for (int j = 0; j < ELEMENTS_AMOUNT; j++)
                vector.push(buffer[j]);

        timer.start();
        for (int j = 0; j < ELEMENTS_AMOUNT; j++) {
            switch (operation) {
                case operation_t::insertion:
                    vector.push(buffer[j]);
                    break;
                case operation_t::search:
                    vector.at(buffer[j]);
                    break;
                case operation_t::deletion:
                    vector.erase(buffer[j]);
                    break;
                default:
                    std::cerr << "Some error occurred when testing vector operation" << std::endl;
                    exit(1);
                    break;
            }
        }
        timer.stop();

        times_buffer[i] = timer.seconds();

        std::cout << i << std::endl;
    }
}

static void list_operation_test (const operation_t& operation, double *times_buffer)
{
    lib::timer_t timer;

    for (int i = 0; i < TESTS_AMOUNT; i++) {
        std::string input_filename("inputs" + std::to_string(i + 1) + ".bin");
        read_input_file_into_buffer(input_filename.c_str());

        yadsl::list_t<int> list;

        if (operation == operation_t::search || operation == operation_t::deletion)
            for (int j = 0; j < ELEMENTS_AMOUNT; j++)
                list.push_back(buffer[j]);

        timer.start();
        for (int j = 0; j < ELEMENTS_AMOUNT; j++) {
            switch (operation) {
                case operation_t::insertion:
                    list.push_back(buffer[j]);
                    break;
                case operation_t::search:
                    list.get(buffer[j]);
                    break;
                case operation_t::deletion:
                    list.erase(list.get(buffer[j]));
                    break;
                default:
                    std::cerr << "Some error occurred when testing vector operation" << std::endl;
                    exit(1);
                    break;
            }
        }
        timer.stop();

        times_buffer[i] = timer.seconds();

        std::cout << i << std::endl;
    }
}

static void vector_tests ()
{
    double insertion_times[TESTS_AMOUNT];
    double search_times[TESTS_AMOUNT];
    double deletion_times[TESTS_AMOUNT];

    vector_operation_test(operation_t::insertion, insertion_times);
    vector_operation_test(operation_t::search, search_times);
    vector_operation_test(operation_t::deletion, deletion_times);

    lib::csvfile_t csv((datasets_path + "vector.csv").c_str());

    csv << "insertion" << "search" << "deletion" << lib::endrow;

    for (uint32_t i = 0; i < TESTS_AMOUNT; i++)
        csv << insertion_times[i] << search_times[i] << deletion_times[i] << lib::endrow;
}

static void list_tests ()
{
    double insertion_times[TESTS_AMOUNT];
    double search_times[TESTS_AMOUNT];
    double deletion_times[TESTS_AMOUNT];

    list_operation_test(operation_t::insertion, insertion_times);
    list_operation_test(operation_t::search, search_times);
    list_operation_test(operation_t::deletion, deletion_times);

    lib::csvfile_t csv((datasets_path + "list.csv").c_str());

    csv << "insertion" << "search" << "deletion" << lib::endrow;

    for (uint32_t i = 0; i < TESTS_AMOUNT; i++)
        csv << insertion_times[i] << search_times[i] << deletion_times[i] << lib::endrow;
}


int main(int argc, char **argv)
{
    // vector_tests();
    list_tests();
}