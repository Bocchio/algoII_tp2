#include <iostream>
#include <iomanip>  // for std::setprecision
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cfloat>
#include <string>

#include "cmdline.hpp"
#include "vector.hpp"
#include "complex.hpp"
#include "errors.hpp"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::fstream;
using std::ios;
using std::istringstream;
using std::string;

static void opt_input1(string const &);
static void opt_input2(string const &);
static void opt_verbose(string const &);

// syntax: {needs argument,
//          short name,
//          long name,
//          default value,
//          parser,
//          flag}
static option_t options[] = {
    {1, "i1", "input1", "-", opt_input1, OPT_DEFAULT},
    {1, "i2", "input2", "-", opt_input2, OPT_DEFAULT},
    {0, "v", "verbose", NULL, opt_verbose, OPT_DEFAULT},
    {0, },
};

static istream *iss1 = 0;
static istream *iss2 = 0;
static fstream ifs1;
static fstream ifs2;
static bool verbose = false;


static void opt_input1(string const &arg) {
    if (arg == "-") {
        iss1 = &cin;
    } else {
        ifs1.open(arg.c_str(), ios::in);
        iss1 = &ifs1;
    }

    if (!iss1->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void opt_input2(string const &arg) {
    if (arg == "-") {
        iss2 = &cin;
    } else {
        ifs2.open(arg.c_str(), ios::in);
        iss2 = &ifs2;
    }

    if (!iss2->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void opt_verbose(string const &arg) {
    verbose = true;
}

int main(int argc, char * const argv[]) {
    cmdline cmdl(options);
    cmdl.parse(argc, argv);

    Vector<Complex> v1, v2;
    if ((*iss1 >> v1).bad()) {
        cerr << ERROR_MSG_CORRUPTED_DATA << endl;
    }
    if ((*iss2 >> v2).bad()) {
        cerr << ERROR_MSG_CORRUPTED_DATA << endl;
    }

    double diff = 0;
    for (size_t i = 0; i < v1.getSize(); i++) {
        diff += abs((v1[i]-v2[i]).getMod());
    }
    std::cout << diff/v1.getSize() << std::endl;

    return 0;
}
