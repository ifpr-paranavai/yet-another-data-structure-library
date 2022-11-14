#include <ctime>
#include <random>
#include <string>
#include <chrono>
#include <iostream>
#include <cinttypes>
#include <functional>
#include <filesystem>
#include <boost/program_options.hpp>

#include "../lib/csvfile.h"

#include "../vector.h"
#include "../list.h"
#include "../binary_tree.h"
#include "../hash_table.h"
#include "../pair.h"

namespace po = boost::program_options;

enum class flags_t
{
    ALL,
    VECTOR,
    LIST,
    BINARY_TREE,
    HASH_TABLE,
};

enum class operation_t
{
    INSERTION,
    SEARCH,
    DELETION,
};

static std::mt19937 rgenerator;
static std::uniform_int_distribution<int> idistribution;

std::filesystem::path current_path = std::filesystem::current_path();
std::string datasets_path = current_path.string() + "/case-study/datasets/";

static uint32_t tests_amount;
static uint32_t numbers_amount;
static flags_t flag;
static char timestamp[80];
static int **random_ints_buffer;
std::function<void(void)> before_each;

static flags_t string_to_flag (const std::string& flag)
{
    if (flag == "all")
        return flags_t::ALL;
    else if (flag == "vector")
        return flags_t::VECTOR;
    else if (flag == "list")
        return flags_t::LIST;
    else if (flag == "binary_tree")
        return flags_t::BINARY_TREE;
    else if (flag == "hash_table")
        return flags_t::HASH_TABLE;
    
    std::cout << "Invalid flag! Type --flags to see the available flags." << std::endl;
    exit(1);
}

static void set_timestamp ()
{
    time_t rawtime;
    tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, 80, "%m_%d_%y_%H_%M_%S", timeinfo);
}

static void setup_program_options (int argc, char **argv)
{
    po::variables_map vm;
    po::options_description desc("Allowed options");

    desc.add_options()
        ("tests_amount,ta", po::value<uint32_t>(), "set the amount of tests to be executed")
        ("numbers_amount,na", po::value<uint32_t>(), "set the amount of numbers generated")
        ("flag,f", po::value<std::string>()->default_value("all"), "flags to define which tests will be executed")
        ("flags", "list of the available flags");

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("flags")) {
        std::cout << "[ FLAGS ]" << std::endl;
        std::cout << "all         |  run all the test suits" << std::endl;
        std::cout << "vector      |  run only the vector's test suits" << std::endl;
        std::cout << "binary_tree |  run only the binary tree's test suits" << std::endl;
        std::cout << "hash_table  |  run only the hash table's test suits" << std::endl;
        exit(0);
    }

    if (!vm.count("tests_amount") || !vm.count("numbers_amount")) {
        std::cerr << "Program options \"tests_amount\" and \"numbers_amount\" are required." << std::endl;
        exit(1);
    }

    tests_amount = vm["tests_amount"].as<uint32_t>();
    numbers_amount = vm["numbers_amount"].as<uint32_t>();
    flag = string_to_flag(vm["flag"].as<std::string>());
    set_timestamp();
}

static void setup_tests ()
{
    random_ints_buffer = new int*[tests_amount];

    std::cout << "**** [ starting tests setup ] ****" << std::endl;
    std::cout << "**** [ generating random values ] ****" << std::endl;

    for (uint32_t i = 0; i < tests_amount; i++)
    {
        rgenerator.seed(i);
        int *random_ints = new int[numbers_amount];

        for (uint32_t j = 0; j < numbers_amount; j++)
        {
            random_ints[j] = idistribution(rgenerator);
        }

        random_ints_buffer[i] = random_ints;
    }

    std::cout << "**** [ testes setup ended ] ****" << std::endl;
}

static void test(double *times_buffer, std::function<void(int)> func, 
    std::function<void(int)> setup = nullptr)
{
    for (uint32_t i = 0; i < tests_amount; i++) {
        int *random_ints = random_ints_buffer[i];

        auto tbegin = std::chrono::steady_clock::now();
        for (uint32_t j = 0; j < numbers_amount; j++) {
            func(random_ints[j]);
        }
        auto tend = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(tend - tbegin);

        times_buffer[i] = elapsed.count();
        std::cout << ".";
    }
    std::cout << std::endl;
}

static void test_vector (operation_t operation, double *times_buffer)
{
    int input_buffer[numbers_amount];
    yadsl::vector_t<int> vector(numbers_amount);

    for (uint32_t i = 0; i < tests_amount; i++) {
        rgenerator.seed(i);

        for (uint32_t j = 0; j < numbers_amount; j++)
            input_buffer[j] = idistribution(rgenerator);
    }

    for (uint32_t i = 0; i < tests_amount; i++) {
        if (operation != operation_t::INSERTION)
            for (uint32_t j = 0; j < numbers_amount; j++)
                vector.push(input_buffer[j]);

        auto tbegin = std::chrono::steady_clock::now();
        for (uint32_t j = 0; j < numbers_amount; j++) {
            switch (operation) {
                case operation_t::INSERTION:
                    vector.push(input_buffer[j]);
                    break;
                case operation_t::SEARCH:
                    try {
                        vector.get(input_buffer[j]);
                    }
                    catch(const char* e) {
                        std::cerr << e << '\n';
                        std::cerr << input_buffer[j] << '\n';
                    }
                    break;
                case operation_t::DELETION:
                    try {
                        vector.erase(input_buffer[j]);
                    }
                    catch(const char* e) {
                        std::cerr << e << '\n';
                        std::cerr << input_buffer[j] << '\n';
                    }
                    break;
                default:
                    std::cerr << "An unexpected error occurred" << std::endl;
                    exit(1);
            }
        }
        auto tend = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(tend - tbegin);

        times_buffer[i] = elapsed.count();
        std::cout << ".";
    }
    std::cout << std::endl;

    
}

static void generate_csv (const std::string& prefix, double *insertion_times, double *search_times, double *deletion_times)
{
    std::string filename = prefix + "_" + timestamp + ".csv";
    lib::csvfile_t csv((datasets_path + filename).c_str());

    std::cout << "/|\\ generating the csv \"" 
        << filename 
        << "\" at " 
        << datasets_path 
        << "/|\\" << std::endl;

    csv << "insertion"
        << "search"
        << "deletion" << lib::endrow;

    for (uint32_t i = 0; i < tests_amount; i++)
        csv << insertion_times[i] 
            << search_times[i] 
            << deletion_times[i] << lib::endrow;
}

static void vector_test_suit ()
{
    if (flag != flags_t::ALL && flag != flags_t::VECTOR) {
        std::cout << "skiping vector test suit" << std::endl;
        return;
    }
    std::cout << "**** [ running vector test suit ] ****" << std::endl;

    double insertion_times[tests_amount];
    double search_times[tests_amount];
    double deletion_times[tests_amount];

    test_vector(operation_t::INSERTION, insertion_times);
    test_vector(operation_t::SEARCH, search_times);
    test_vector(operation_t::DELETION, deletion_times);

    generate_csv("vector", insertion_times, search_times, deletion_times);
}

static void list_test_suit ()
{
    if (flag != flags_t::ALL && flag != flags_t::LIST) {
        std::cout << "skiping list test suit" << std::endl;
        return;
    }
    std::cout << "**** [ running list test suit ] ****" << std::endl;

    yadsl::list_t<int> *list = nullptr;
    std::function<void(int)> setup_test = [&](int random_int) {
        list->push_back(random_int);
    };

    double insertion_times[tests_amount];
    double search_times[tests_amount];
    double deletion_times[tests_amount];

    before_each = [&]() {
        if (list != nullptr)
            delete list;
        list = new yadsl::list_t<int>();
    };

    std::cout << "INSERTION" << std::endl;
    test(insertion_times, [&](int random_int){
        list->push_back(random_int);
    });
    std::cout << "SEARCH" << std::endl;
    test(search_times, [&](int random_int){
        list->get(random_int);
    }, setup_test);
    std::cout << "DELETION" << std::endl;
    test(deletion_times, [&](int random_int){
        list->erase(list->get(random_int));
    }, setup_test);

    generate_csv("list", insertion_times, search_times, deletion_times);
}

static void binary_tree_test_suit ()
{
    if (flag != flags_t::ALL && flag != flags_t::BINARY_TREE) {
        std::cout << "skiping binary tree test suit" << std::endl;
        return;
    }
    std::cout << "**** [ running binary tree test suit ] ****" << std::endl;

    yadsl::binary_tree_t<int> *tree = nullptr;
    std::function<void(int)> setup_test = [&](int random_int) {
        tree->add(random_int);
    };

    double insertion_times[tests_amount];
    double search_times[tests_amount];
    double deletion_times[tests_amount];

    before_each = [&]() {
        if (tree != nullptr)
            delete tree;
        tree = new yadsl::binary_tree_t<int>();
    };

    std::cout << "INSERTION" << std::endl;
    test(insertion_times, [&](int random_int){
        tree->add(random_int);
    });
    std::cout << "SEARCH" << std::endl;
    test(search_times, [&](int random_int){
        tree->get(random_int);
    }, setup_test);
    std::cout << "DELETION" << std::endl;
    test(deletion_times, [&](int random_int){
        tree->erase(random_int);
    }, setup_test);

    generate_csv("binary_tree", insertion_times, search_times, deletion_times);
}

static void hash_table_test_suit ()
{
    if (flag != flags_t::ALL && flag != flags_t::HASH_TABLE) {
        std::cout << "skiping hash table test suit" << std::endl;
        return;
    }
    std::cout << "**** [ running hash table test suit ] ****" << std::endl;

    using pair_t = yadsl::pair_t<int, int>;
    yadsl::hash_table_t<int, int> *hash_table = nullptr;
    std::function<void(int)> setup_test = [&](int random_int) {
        hash_table->add(pair_t(random_int, random_int));
    };

    double insertion_times[tests_amount];
    double search_times[tests_amount];
    double deletion_times[tests_amount];

    before_each = [&]() {
        if (hash_table != nullptr)
            delete hash_table;
        hash_table = new yadsl::hash_table_t<int, int>(numbers_amount % 2);
    };

    std::cout << "INSERTION" << std::endl;
    test(insertion_times, [&](int random_int){
        hash_table->add(pair_t(random_int, random_int));
    });
    std::cout << "SEARCH" << std::endl;
    test(search_times, [&](int random_int){
        hash_table->get(random_int);
    }, setup_test);
    std::cout << "DELETION" << std::endl;
    test(deletion_times, [&](int random_int){
        hash_table->erase(random_int);
    }, setup_test);

    generate_csv("hash_table", insertion_times, search_times, deletion_times);
}

static void run_tests ()
{
    std::cout << "**** [ starting to run the tests ] ****" << std::endl;
    vector_test_suit();
    list_test_suit();
    binary_tree_test_suit();
    hash_table_test_suit();
}

int main (int argc, char **argv)
{
    setup_program_options(argc, argv);
    setup_tests();
    run_tests();
    return 0;
}