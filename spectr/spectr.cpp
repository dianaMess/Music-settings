#include<iostream>
#include<vector>
#include "fft.h"
using namespace std;
int power(int n) {
    int d = 1;
    while (d < n)
        d *= 2;
    return d;
}
 
int main(int argc, char * argv[]) {
    int factor = atof(argv[1]);
    std::ifstream input(argv[2], std::ios_base::binary);
    std::istreambuf_iterator<char> start(input);
    std::istreambuf_iterator<char> end;
    std::vector<char> buf(start, end);
    vector<Complex> comp = parse(buf, 0);
    vector<Complex> comp1 = parse(buf, 1);
    while (comp.size() != power(comp.size())) {
        comp.push_back(Complex(0, 0));
        comp1.push_back(Complex(0, 0));
    }
    std::vector<Complex> spectr2 = fft(comp, comp.size(), 1);
    vector<Complex> vec(spectr2.size());
    int n = spectr2.size();
    for (int i = 0; i < n / 2; i++) {
        vec[i] = spectr2[(int)(i * factor)];
        vec[n - 1 - i] = spectr2[(int)((n - 1 - i) * factor)];
    }
    vector<Complex> rev = fft(vec, vec.size(), 0); 
    write_to_chanel(rev, rev, spectr2.size(), buf); 
    return 0;
}
