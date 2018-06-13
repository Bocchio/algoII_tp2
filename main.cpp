#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cfloat>
#include <string>

#include <ctime>  // for timing purposes
#include <vector>
#include <complex>

#include "cmdline.hpp"
#include "vector.hpp"
#include "complex.hpp"
#include "ft.hpp"
#include "errors.hpp"
#include "dictionary.hpp"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::fstream;
using std::ios;
using std::istringstream;
using std::string;

static void opt_input(string const &);
static void opt_output(string const &);
static void opt_method(string const &);
static void opt_regression(string const &);
static void opt_error(string const &);
static void opt_help(string const &);

// syntax: {needs argument,
//          short name,
//          long name,
//          default value,
//          parser,
//          flag}
static option_t options[] = {
    {1, "i", "input", "-", opt_input, OPT_DEFAULT},
    {1, "o", "output", "-", opt_output, OPT_DEFAULT},
    {1, "m", "method", "FFT", opt_method, OPT_DEFAULT},
    {1, "r", "regression", NULL, opt_regression, OPT_DEFAULT},
    {1, "e", "error", NULL, opt_error, OPT_DEFAULT},
    {0, "h", "help", NULL, opt_help, OPT_DEFAULT},
    {0, },
};

static istream *iss = 0;
static ostream *oss = 0;
template<typename Derived>
FourierTransform * constructorFrom() { return new Derived; }
static fstream ifs;
static fstream ofs;
FourierTransform * FT;
Dictionary<FourierTransform *(*)()> method_lookup_table;

static void opt_input(string const &arg) {
    if (arg == "-") {
        iss = &cin;
    } else {
        ifs.open(arg.c_str(), ios::in);
        iss = &ifs;
    }

    if (!iss->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void opt_output(string const &arg) {
    if (arg == "-") {
        oss = &cout;
    } else {
        ofs.open(arg.c_str(), ios::out);
        oss = &ofs;
    }

    if (!oss->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void opt_method(const string& method) {
    method_lookup_table["FFT"] = constructorFrom<FFT>;
    method_lookup_table["IFFT"] = constructorFrom<IFFT>;
    method_lookup_table["DFT"] = constructorFrom<DFT>;
    method_lookup_table["IDFT"] = constructorFrom<IDFT>;

    FT = method_lookup_table[method]();
}

static void opt_regression(string const &arg) {
    ;
}

static void opt_error(string const &arg) {
    ;
}

static void opt_help(string const &arg) {
    cout << HELP_MSG
         << endl;
    exit(0);
}

int main(int argc, char * const argv[]) {
    cmdline cmdl(options);
    cmdl.parse(argc, argv);

    Vector<Complex> v;

    if ((*iss >> v).bad()) {
        cerr << ERROR_MSG_CORRUPTED_DATA << endl;
    }

    v = FT->transform(v);

    // if (regression) {
    //     double relative_error = 0;
    //     double regression_square_sum = 0;
    //     for (size_t i = 0; i < regressions.getSize(); i++) {
    //         square_difference = (v[i]-regressions[i]);
    //         square_difference = square_difference*square_difference.getConjugate();

    //         regression_square_sum += regressions[i]*regressions[i].getConjugate();

    //         relative_error += square_difference;
    //     }
    //     relative_error = sqrt(relative_error/regression_square_sum);

    //     if (relative_error < error) {
    //         exit(0);
    //     }
    // }

    delete FT;

    return 0;
}
