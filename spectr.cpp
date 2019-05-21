#include<iostream>
#include<vector>
#include "src/fft.h"
using namespace std;
int power(int n) {
    int d = 1;
    while (d < n)
        d *= 2;
    return d;
}
void change_spectr(vector<Complex> sound, double factor, double sec, vector<char> buf) {
    int step = 44100 * sec, size = sound.size(), i = 0;
    vector<Complex> new_vec;
    while (size > 0) {
        vector<Complex> sound1(sound.begin() + step * i, sound.begin() + step * i + step);
        while (sound1.size() != power(sound1.size()))
            sound1.push_back(Complex(0, 0));
        std::vector<Complex> spectr2 = fft(sound1, sound1.size(), 1);
        vector<Complex> vec(spectr2.size());
        int n = spectr2.size();
        for (int i = 0; i < n / 2; i++) {
            vec[i] = spectr2[(int)(i * factor)];
            vec[n - 1 - i] = vec[i];
        }
        vector<Complex> rev = fft(vec, vec.size(), 0); 
        size = size - step;
        i++;
        for (int i = 0; i < rev.size(); i++)
            new_vec.push_back(rev[i]);
    }
    write_to_chanel(new_vec, new_vec.size(), buf); 
} 
int main(int argc, char * argv[]) {
    double factor = atof(argv[1]);
    double sec= atof(argv[2]);
    std::ifstream input(argv[3], std::ios_base::binary);
    std::istreambuf_iterator<char> start(input);
    std::istreambuf_iterator<char> end;
    std::vector<char> buf(start, end);
    vector<Complex> comp = parse(buf);
    while (comp.size() != power(comp.size())) {
        comp.push_back(Complex(0, 0));
    }
    change_spectr(comp, factor, sec, buf);
    return 0;
}
