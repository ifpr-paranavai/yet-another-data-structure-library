#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

static std::string filename;
static po::variables_map vm;
static po::options_description desc("Allowed options");
static std::ofstream ofile;
static uint32_t numbers_amount = 1000000;

static std::random_device rd;
static std::mt19937_64 rgenerator;
static std::uniform_int_distribution<int> idistribution(0, numbers_amount - 1);
static std::uniform_real_distribution<double> rdistribution(0.0, 1.0);

static void setup_program_options (int argc, char **argv, po::variables_map& vm)
{
    desc.add_options()
        ("filename,f", po::value<std::string>(), "set the file name")
        ("numbers_amount,na", po::value<uint32_t>()->default_value(1000000), "set the amount of numbers generated");

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
}

static void generate_input_file (const std::string& filename, uint32_t numbers_amount)
{
    int buffer[numbers_amount];
    rgenerator.seed(rd());

    std::cout << numbers_amount << std::endl;

    for (int i = 0; i < numbers_amount; i++)
        buffer[i] = idistribution(rgenerator);

    FILE *fp = fopen(filename.c_str(), "wb");
    fwrite((void *)buffer, sizeof(int), numbers_amount, fp);
    fclose(fp);
}

int main(int argc, char **argv)
{
    setup_program_options(argc, argv, vm);

    if (vm.count("filename"))
        filename = vm["filename"].as<std::string>();
    else {
        std::cerr << "Program option \"filename\" is required." << std::endl;
        exit(1);
    }

    if (vm.count("numbers_amount")) {
        numbers_amount = vm["numbers_amount"].as<uint32_t>();
        idistribution = std::uniform_int_distribution<int>(0, numbers_amount - 1);
    }

    std::cout << "Generating input file: " << filename.c_str() << ".bin." << std::endl;

    std::filesystem::path current_path = std::filesystem::current_path();
    generate_input_file(current_path.string() + "/case-study/input-files/" + filename + ".bin", numbers_amount);

    std::cout << "File successfully generated at ./case-study/input-files/" << filename.c_str() << ".bin." << std::endl;

    return 0;
}
