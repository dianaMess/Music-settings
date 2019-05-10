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
 
int main() {
    std::ifstream input("../examples/07070057.wav", std::ios_base::binary);
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
    cout<<"done"<<endl;
    std::vector<Complex> spectr2 = fft(comp, comp.size(), 0);
    std::vector<Complex> spectr3 = fft(comp1, comp1.size(), 0);
    cout<<"task"<<endl;
//----TASK 2----
//change spectr
    vector<Complex> vec(spectr2.size());
    vector<Complex> vec1(spectr3.size());
    int n = spectr2.size();
    cout<<"start"<<endl;
    for (int i = 0; i < n / 2; i++) {
        vec[i] = spectr2[(int)(i * 0.5)];
        vec1[i] = spectr3[(int)(i * 0.5)];
        vec[n - 1 - i] = spectr2[(int)((n - 1 - i) * 0.5)];
        vec1[n - 1 - i] = spectr3[(int)((n - 1 - i) * 0.5)];
    }
    vector<Complex> rev = fft(vec, vec.size(), 1); 
    vector<Complex> rev1 = fft(vec1, vec1.size(), 1); 
    write_to_chanel(rev, rev1, spectr2.size(), buf); 

    return 0;
}
