#include<iostream>
#include<vector>
#include "fft.h"
int power(int n) {
    int d = 1;
    while (d < n)
        d *= 2;
    return d;
}
//1-st argument is file
//2-nd sec/in block
//3-rd factor
using namespace std;
int main(int argc, char * argv[]) {
    std::ifstream input("07070174.wav", std::ios_base::binary);
    std::istreambuf_iterator<char> start(input);
    std::istreambuf_iterator<char> end;
    std::vector<char> buf(start, end);
//    cout<<buf.size()<<endl;
    vector<Complex> comp = parse(buf, 0);
    vector<Complex> comp1 = parse(buf, 1);
    while (comp.size() != power(comp.size())) {
        comp.push_back(Complex(0, 0));
        comp1.push_back(Complex(0, 0));
    }
    std::vector<Complex> spectr2 = fft(comp, comp.size(), 0);
    std::vector<Complex> spectr3 = fft(comp1, comp1.size(), 0);
//----TASK 1----
    
    double time = 0.01;
    int p = 0.5;
    double max = 0.0;
    int prev = 0, step = 44100 * time, count = 0; 
    cout<<step<<endl;
    for (int i = 0; i < spectr2.size(); i++) {
       if (i % step != 0) {
            if (spectr2[i].amp() > max) 
                max = spectr2[i].amp();
       } else {
            count++;
            int len = i - prev;
            for (int j = prev; j < len; j++) {
                if (spectr2[j].amp() > p * max) {
                    cout<<44100.0 / spectr2.size() * j<<endl;
                }
            }
            max = 0.0;
            prev = i;
        }
    }
    return 0;
}
