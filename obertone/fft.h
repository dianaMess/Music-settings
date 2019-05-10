#ifndef FFT_H
#define FFT_H
#include<iostream>
#include<vector>
#include<cmath>
#include<fstream>

using namespace std;
class Complex {
    double Re;
    double Im;
public:
    Complex(double re = 0, double im = 0): Re(re), Im(im) {}
    Complex operator+(const Complex &cmp);
    Complex operator-(const Complex &cmp);
    Complex operator*(const Complex &cmp);
    Complex operator+=(const Complex &cmp);
    Complex operator=(const Complex & cmp);
    Complex operator/(double d);
    Complex operator*(double d);
    Complex operator^(double d);
    double amp();
    double re();
    friend istream& operator>>(istream &in, Complex &cmp) {
      cin>>cmp.Re>>cmp.Im;
      return in;
    }
    friend std::ostream& operator<<(std::ostream &out, Complex cmp) {
        out<<cmp.Re<<' '<<cmp.Im<<endl;
        return out;
    }
       
};
std::vector<Complex> fft(std::vector<Complex> a, int n, int switch_fft);
std::vector<Complex> parse(std::vector<char> buf, bool chanel);
void write_to_chanel(std::vector<Complex> spectrl, std::vector<Complex> spectrr,
                     int size, std::vector<char> buf);
#endif
