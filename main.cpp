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

#define FLOAT_DIGITS 7
#define DOUBLE_DIGITS 16

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
    {1, "r", "regression", "", opt_regression, OPT_DEFAULT},
    {1, "e", "error", "1e-3", opt_error, OPT_DEFAULT},
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

static istream *regressions_stream = 0;
static fstream regressions_file_stream;
static bool using_regression_file;
static float error_threshold;

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
    method_lookup_table.add("FFT", constructorFrom<FFT>);
    method_lookup_table.add("IFFT", constructorFrom<IFFT>);
    method_lookup_table.add("DFT", constructorFrom<DFT>);
    method_lookup_table.add("IDFT", constructorFrom<IDFT>);

    FT = method_lookup_table[method]();
}

static void opt_regression(string const &arg) {
    if (arg == "") {
        using_regression_file = false;
        return;
    } else {
        regressions_file_stream.open(arg.c_str(), ios::in);
        regressions_stream = &regressions_file_stream;
        using_regression_file = true;
    }
    if (!regressions_stream->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << arg
             << "."
             << endl;
        exit(1);
    }
}

static void opt_error(string const &arg) {
    char *tmp;

    error_threshold = strtod(arg.c_str(), &tmp);
    if(*tmp) {
        cout << ERROR_MSG_INVALID_ARG
             << arg
             << endl;
        exit(1);
    }
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

    size_t exit_code = EXIT_SUCCESS;

    if (using_regression_file) {
        size_t test_number = 1;
        string test_result;
        while (*regressions_stream) {
            Vector<Complex> v;
            Vector<Complex> regressions;
            if ((*iss >> v).bad()) {
                cerr << ERROR_MSG_CORRUPTED_DATA << endl;
            }
            if ((*regressions_stream >> regressions).bad()) {
                cerr << ERROR_MSG_CORRUPTED_DATA << endl;
            }
            if (v.getSize() == 0)
                break;

            v = FT->transform(v);

            double relative_error = 0;
            double regression_square_sum = 0;
            double square_difference = 0;
            for (size_t i = 0; i < regressions.getSize(); i++) {
                square_difference = ((v[i]-regressions[i])*((v[i]-regressions[i]).getConjugate()))
                                    .getReal();

                regression_square_sum += (regressions[i]*(regressions[i].getConjugate()))
                                         .getReal();

                relative_error += square_difference;
            }
            relative_error = sqrt(relative_error/regression_square_sum);

            if (relative_error > error_threshold) {
                test_result = "error";
                exit_code = EXIT_FAILURE;
            } else {
                test_result = "ok";
            }

            *oss << "test " << test_number << ": " <<
                               test_result << " " <<
                               v.getSize() << " " <<
                               relative_error << endl;
            test_number++;
        }
    } else {
        Vector<Complex> v;
        if ((*iss >> v).bad())
            cerr << ERROR_MSG_CORRUPTED_DATA << endl;
        *oss << std::setprecision(FLOAT_DIGITS) << std::fixed << FT->transform(v) << endl;
    }

    delete FT;

    return exit_code;
}
