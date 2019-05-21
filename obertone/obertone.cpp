#include<iostream>
#include<vector>
#include "fft.h"
int power(int n) {
    int d = 1;
    while (d < n)
        d *= 2;
    return d;
}
void find_obertones(vector<Complex> spectr2, double factor, double sec) {
    double max = 0.0;
    int prev = 0, step = 44100 * sec, count = 0; 
    for (int i = 0; i < spectr2.size(); i++) {
       if (i % step != 0) {
            if (spectr2[i].amp() > max) 
                max = spectr2[i].amp();
       } else {
            count++;
            int len = i - prev;
            for (int j = prev; j < len; j++) {
                if (spectr2[j].amp() > factor * max) {
                    cout<<44100.0 / spectr2.size() * j<<endl;
                }
            }
            max = 0.0;
            prev = i;
        }
    }
    return;
}
using namespace std;
int main(int argc, char * argv[]) {
    double factor = atof(argv[1]);
    double sec = atof(argv[2]);
    std::ifstream input(argv[3], std::ios_base::binary);
    std::istreambuf_iterator<char> start(input);
    std::istreambuf_iterator<char> end;
    std::vector<char> buf(start, end);
    vector<Complex> comp = parse(buf);
    while (comp.size() != power(comp.size())) {
        comp.push_back(Complex(0, 0));
    }
    std::vector<Complex> spectr2 = fft(comp, comp.size(), 1);
    find_obertones(spectr2, factor, sec);
    return 0;
}
