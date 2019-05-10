#include<iostream>
#include<vector>
#include<cmath>
#include "fft.h"
using namespace std;
    Complex Complex::operator+(const Complex &cmp) {
        Complex tmp(Re + cmp.Re, Im + cmp.Im);
        return tmp;
    }
    Complex Complex::operator-(const Complex &cmp) {
        Complex tmp(Re - cmp.Re, Im - cmp.Im);
        return tmp;
    }
 
    Complex Complex::operator*(const Complex &cmp) {
        Complex tmp(Re*cmp.Re - Im * cmp.Im, Re * cmp.Im + Im * cmp.Re);
        return tmp;
    }
    Complex Complex::operator+=(const Complex &cmp) {
        Complex tmp(Re + cmp.Re, Im + cmp.Im);
        return tmp;
    }
    Complex Complex::operator=(const Complex & cmp) {
        Re = cmp.Re;
        Im = cmp.Im;
        return *this;
    }
    Complex Complex::operator/(double d) {
        Complex tmp(Re / d, Im / d);
        return tmp;
    }
    Complex Complex::operator*(double d) {
        Complex tmp(Re * d, Im * d);
        return tmp;
    }
    Complex Complex::operator^(double d) {
        Complex tmp(Re * cos(d) - Im * sin(d), Im * cos(d) + Re * sin(d));
        return tmp;
    }
    double Complex::amp() {
        double a = sqrt(Re * Re + Im * Im);
        return a;
    }
    double Complex::re() {
        return Re;
    }
/*    friend istream& operator>>(istream &in, Complex &cmp) {
        cin>>cmp.Re>>cmp.Im;
        return in;
    }

    friend std::ostream& operator<<(std::ostream &out, Complex cmp) {
        out<<cmp.Re<<' '<<cmp.Im<<endl;
        return out;
    }
 */

std::vector<Complex> fft(std::vector<Complex> a, int n, int switch_fft) {
    if (n <= 1)
        return a; 
    int m = 0, k = 0;
    Complex wn(cos(2 * (M_PI/n)), sin(2 * (M_PI/n))), w(1.0, 0.0);
    std::vector<Complex> a0(n/2), a1(n/2);
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0)
            a0[k++] = a[i];
        else 
            a1[m++] = a[i];
    }
    std::vector<Complex> y0, y1, y(n);
    y0 = fft(a0, n / 2, switch_fft);
    y1 = fft(a1, n / 2, switch_fft);
    for (int j = 0; j < n/2; j++) {
       Complex tmp;
       tmp = y1[j] * w;
       y[j] = y0[j] + tmp;
       y[j + (n/2)] = y0[j] - tmp;
       if (switch_fft) {
           y[j] = y[j] / 2;
           y[j + n/2] = y[j + n/2] / 2;
       }
       w = w * wn;
    }
    return y;
}
    std::vector<Complex> parse(std::vector<char> buf, bool chanel) {
        std::vector<Complex> function;
        int chunk_start = 0;
        std::string chunk_name(buf.begin() + chunk_start, buf.begin() + chunk_start + 4);
        int chunk_size = *(int *)(buf.data() + chunk_start);
        chunk_start += 4;

        while (chunk_name != "data") {
            chunk_name = std::string(buf.begin() + chunk_start, buf.begin() + chunk_start + 4);
            chunk_start += 4;
        }
        chunk_start += 4;
        chunk_size = buf.size() - chunk_start;
        for (int i = 0 ; i < chunk_size; i += 4) {
            short value = *(short *)(buf.data() + i + chunk_start + chanel * 2);
            function.push_back(value);
        }
        return function;
    }
     void write_to_chanel(std::vector<Complex> spectrl, std::vector<Complex> spectrr,
                         int size, std::vector<char> buf) {
        int shift = 0;
        std::ofstream output("out.wav", std::ios::out | std::ios::binary);
        std::vector<char> buf2 = buf;
        for (int i = 0; i < 768; i++)
            buf2[i] = buf[i];
        int len = 0;
        for (int i = 0; i < size; i += 4) {
            *(short *)&buf2[768 + i] = spectrl[i].re();
            *(short *)&buf2[768 + i + 2] = spectrr[i].re();
            len = i + 2;
//            *(short *)&buf2[768 + 4 * shift + 4 * i] = spectrl[i].re();
//            *(short *)&buf2[768 + 4 * shift + 4 * i + 2] = spectrr[i].re();
        }
        output.write(buf2.data(), len);
        output.close();
    }
 
