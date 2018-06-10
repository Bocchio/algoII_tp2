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

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::fstream;
using std::ios;
using std::istringstream;
using std::string;

// syntax: {needs argument,
//          short name,
//          long name,
//          default value,
//          parser,
//          flag}
static option_t options[] = {
    {0, },
};

static ostream *fft_oss = 0;
static fstream fft_ofs;
static ostream *dft_oss = 0;
static fstream dft_ofs;
static const char *fft_filename = "datosFFT.txt";
static const char *dft_filename = "datosDFT.txt";

int main(int argc, char * const argv[]) {
    cmdline cmdl(options);
    cmdl.parse(argc, argv);

    FourierTransform *fft = new FFT();
    FourierTransform *dft = new DFT();

    fft_ofs.open(fft_filename, ios::out);
    fft_oss = &fft_ofs;
    if (!fft_oss->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << fft_filename
             << "."
             << endl;
        exit(1);
    }
    dft_ofs.open(dft_filename, ios::out);
    dft_oss = &dft_ofs;
    if (!dft_oss->good()) {
        cerr << ERROR_MSG_CANT_OPEN_FILE
             << dft_filename
             << "."
             << endl;
        exit(1);
    }

    int max_size = 1000;
    float division_constant = 7;
    for (size_t i = 1; i < 16385; i *= 2) {
        Vector<Complex> v;
        for (size_t j = 0, r = 1; j < i; j++) {
            srand((j+i+r)*(j+i+r));
            r = rand();
            srand((j+i-r)*(j+i-r));
            v.append(Complex((r%max_size)/division_constant,
                             (rand()%max_size)/division_constant));
        }
        clock_t begin = clock();
        fft->transform(v);
        clock_t end = clock();
        double elapsed_secs = double(end - begin)/CLOCKS_PER_SEC;
        *fft_oss << i << " " << elapsed_secs << std::endl;

        begin = clock();
        dft->transform(v);
        end = clock();
        elapsed_secs = double(end - begin)/CLOCKS_PER_SEC;
        *dft_oss << i << " " << elapsed_secs << std::endl;

        std::cout << i << std::endl;
    }

    return 0;
}
