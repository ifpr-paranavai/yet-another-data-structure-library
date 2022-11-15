#include <ctime>
#include <random>
#include <string>
#include <chrono>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>
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
#include "../lib.h"

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
static std::uniform_int_distribution<uint64_t> idistribution;

std::filesystem::path current_path = std::filesystem::current_path();
std::string datasets_path = current_path.string() + "/datasets/";

static uint64_t tests_amount;
static uint64_t numbers_amount;
static flags_t flag;
static char timestamp[80];
static std::vector<uint64_t> *random_ints_buffer;
static std::vector<uint64_t> *random_ints_buffer_shuffle;
std::function<void(void)> before_each;
static uint64_t foo = 0;

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
        ("tests_amount,ta", po::value<uint64_t>()->default_value(30), "set the amount of tests to be executed")
        ("numbers_amount,na", po::value<uint64_t>()->default_value(2000000), "set the amount of numbers generated")
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

    tests_amount = vm["tests_amount"].as<uint64_t>();
    numbers_amount = vm["numbers_amount"].as<uint64_t>();
    flag = string_to_flag(vm["flag"].as<std::string>());
    set_timestamp();

    std::cout << "Tests amount = " << tests_amount << std::endl;
    std::cout << "Numbers amount = " << numbers_amount << std::endl;
    std::cout << "Flag = " << vm["flag"].as<std::string>() << std::endl;
}

static void setup_tests ()
{
    random_ints_buffer = new std::vector<uint64_t>[tests_amount];
    random_ints_buffer_shuffle = new std::vector<uint64_t>[tests_amount];

    std::cout << "**** [ starting tests setup ] ****" << std::endl;
    std::cout << "**** [ generating random values ] ****" << std::endl;

    for (uint64_t i = 0; i < tests_amount; i++) {
        rgenerator.seed(i);
        std::vector<uint64_t>& random_ints = random_ints_buffer[i];
        std::vector<uint64_t>& random_ints_shuffle = random_ints_buffer_shuffle[i];

        random_ints.reserve(numbers_amount);

        for (uint64_t j = 0; j < numbers_amount; j++)
            random_ints.push_back(idistribution(rgenerator));

        random_ints_shuffle = random_ints;
        std::shuffle(random_ints_shuffle.begin(), random_ints_shuffle.end(), rgenerator);
    }

    std::cout << "**** [ testes setup ended ] ****" << std::endl;
}

static void test(double *times_buffer, std::function<void(uint64_t)> func, 
    std::function<void(uint64_t)> setup = nullptr, bool shuffle = false)
{
    for (uint64_t i = 0; i < tests_amount; i++) {
        std::vector<uint64_t>& random_ints = random_ints_buffer[i];
        std::vector<uint64_t>& random_ints_shuffle = random_ints_buffer_shuffle[i];

        if (before_each != nullptr)
            before_each();

        if (setup != nullptr) {
            for (uint64_t j = 0; j < numbers_amount; j++)
                setup(random_ints[j]);
        }

        auto tbegin = std::chrono::steady_clock::now();
        if (shuffle)
            for (uint64_t j = 0; j < numbers_amount; j++)
                func(random_ints_shuffle[j]);
        else
            for (uint64_t j = 0; j < numbers_amount; j++)
                func(random_ints[j]);
        auto tend = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(tend - tbegin);

        times_buffer[i] = elapsed.count();
    }
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

    for (uint64_t i = 0; i < tests_amount; i++)
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

    yadsl::vector_t<uint64_t> *vector = nullptr;
    std::function<void(uint64_t)> setup_test = [&](uint64_t random_int) {
        vector->push(random_int);
    };

    double insertion_times[tests_amount];
    double search_times[tests_amount];
    double deletion_times[tests_amount];

    before_each = [&]() {
        if (vector != nullptr)
            delete vector;
        vector = new yadsl::vector_t<uint64_t>(numbers_amount);
    };

    std::cout << "INSERTION" << std::endl;
    test(insertion_times, [&](uint64_t random_int){
        vector->push(random_int);
    });
    std::cout << "SEARCH" << std::endl;
    test(search_times, [&](uint64_t random_int){
        foo += vector->get(random_int);
    }, setup_test, true);
    std::cout << "DELETION" << std::endl;
    test(deletion_times, [&](uint64_t random_int){
        vector->erase(random_int);
    }, setup_test, true);

    generate_csv("vector", insertion_times, search_times, deletion_times);
}

static void list_test_suit ()
{
    if (flag != flags_t::ALL && flag != flags_t::LIST) {
        std::cout << "skiping list test suit" << std::endl;
        return;
    }
    std::cout << "**** [ running list test suit ] ****" << std::endl;

    #if DEBUG_MODE
        stats_t<uint64_t> stats(numbers_amount);
    #endif
    yadsl::list_t<uint64_t> *list = nullptr;
    std::function<void(uint64_t)> setup_test = [&](uint64_t random_int) {
        list->push_back(random_int);
    };

    double insertion_times[tests_amount];
    double search_times[tests_amount];
    double deletion_times[tests_amount];

    before_each = [&]() {
        if (list != nullptr)
            delete list;
        list = new yadsl::list_t<uint64_t>();
    };

    #if DEBUG_MODE
        std::cout << "SEARCH" << std::endl;
        test(search_times, [&](uint64_t random_int){
            auto node = list->get(random_int, stats);
            assert(node != nullptr);
        }, setup_test);
    #else
        std::cout << "INSERTION" << std::endl;
        test(insertion_times, [&](uint64_t random_int){
            list->push_back(random_int);
        });
        std::cout << "SEARCH" << std::endl;
        test(search_times, [&](uint64_t random_int){
            foo += list->get(random_int) != nullptr;
        }, setup_test, true);
        std::cout << "DELETION" << std::endl;
        test(deletion_times, [&](uint64_t random_int){
            list->erase(list->get(random_int));
        }, setup_test, true);
    #endif

    #if DEBUG_MODE
        std::cout << "MEAN: " << stats.mean() << std::endl;
        std::cout << "STDDEV: " << stats.stddev() << std::endl;
    #endif

    generate_csv("list", insertion_times, search_times, deletion_times);
}

static void binary_tree_test_suit ()
{
    if (flag != flags_t::ALL && flag != flags_t::BINARY_TREE) {
        std::cout << "skiping binary tree test suit" << std::endl;
        return;
    }
    std::cout << "**** [ running binary tree test suit ] ****" << std::endl;

    yadsl::binary_tree_t<uint64_t> *tree = nullptr;
    std::function<void(uint64_t)> setup_test = [&](uint64_t random_int) {
        tree->add(random_int);
    };

    double insertion_times[tests_amount];
    double search_times[tests_amount];
    double deletion_times[tests_amount];

    before_each = [&]() {
        if (tree != nullptr)
            delete tree;
        tree = new yadsl::binary_tree_t<uint64_t>();
    };

    std::cout << "INSERTION" << std::endl;
    test(insertion_times, [&](uint64_t random_int){
        tree->add(random_int);
    });
    std::cout << "SEARCH" << std::endl;
    test(search_times, [&](uint64_t random_int){
        foo += tree->get(random_int) != nullptr;
    }, setup_test, true);
    std::cout << "DELETION" << std::endl;
    test(deletion_times, [&](uint64_t random_int){
        tree->erase(random_int);
    }, setup_test, true);

    generate_csv("binary_tree", insertion_times, search_times, deletion_times);
}

static void hash_table_test_suit ()
{
    if (flag != flags_t::ALL && flag != flags_t::HASH_TABLE) {
        std::cout << "skiping hash table test suit" << std::endl;
        return;
    }
    std::cout << "**** [ running hash table test suit ] ****" << std::endl;

    #if DEBUG_MODE
        stats_t<uint64_t> stats(numbers_amount);
    #endif
    yadsl::hash_table_t<uint64_t, uint64_t> *hash_table = nullptr;
    std::function<void(uint64_t)> setup_test = [&](uint64_t random_int) {
        hash_table->add(random_int, random_int);
    };

    double insertion_times[tests_amount];
    double search_times[tests_amount];
    double deletion_times[tests_amount];

    before_each = [&]() {
        if (hash_table != nullptr)
            delete hash_table;
        hash_table = new yadsl::hash_table_t<uint64_t, uint64_t>(numbers_amount);
    };

    #if DEBUG_MODE
        std::cout << "SEARCH" << std::endl;
        test(search_times, [&](uint64_t random_int){
            auto pair = hash_table->get(random_int, stats);
            assert(pair.get_key() != -1);
        }, setup_test);
    #else
        std::cout << "INSERTION" << std::endl;
        test(insertion_times, [&](uint64_t random_int){
            hash_table->add(random_int, random_int);
        });
        std::cout << "SEARCH" << std::endl;
        test(search_times, [&](uint64_t random_int){
            foo += hash_table->get(random_int).get_key();
        }, setup_test, true);
        std::cout << "DELETION" << std::endl;
        test(deletion_times, [&](uint64_t random_int){
            hash_table->erase(random_int);
        }, setup_test, true);
    #endif

    #if DEBUG_MODE
        std::cout << "MEAN: " << stats.mean() << std::endl;
        std::cout << "STDDEV: " << stats.stddev() << std::endl;
    #endif

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
    std::cout << "foo: " << foo << std::endl;
    return 0;
}