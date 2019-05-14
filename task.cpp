#include<iostream>
#include<cmath>
#include<vector>
#include<fstream>
using namespace std;

class Complex {
    double Re;
    double Im;
public:
    Complex(double re = 0, double im = 0): Re(re), Im(im) {}
    Complex operator+(const Complex &cmp) {
        Complex tmp(Re + cmp.Re, Im + cmp.Im);
        return tmp;
    }
    Complex operator-(const Complex &cmp) {
        Complex tmp(Re - cmp.Re, Im - cmp.Im);
        return tmp;
    }
 
    Complex operator*(const Complex &cmp) {
        Complex tmp(Re*cmp.Re - Im * cmp.Im, Re * cmp.Im + Im * cmp.Re);
        return tmp;
    }
    Complex operator+=(const Complex &cmp) {
        Complex tmp(Re + cmp.Re, Im + cmp.Im);
        return tmp;
    }
    Complex operator=(const Complex & cmp) {
        Re = cmp.Re;
        Im = cmp.Im;
        return *this;
    }
    Complex operator/(double d) {
        Complex tmp(Re / d, Im / d);
        return tmp;
    }
    Complex operator*(double d) {
        Complex tmp(Re * d, Im * d);
        return tmp;
    }
    Complex operator^(double d) {
        Complex tmp(Re * cos(d) - Im * sin(d), Im * cos(d) + Re * sin(d));
        return tmp;
    }
    double amp() {
        double a = sqrt(Re * Re + Im * Im);
        return a;
    }
    double re() {
        return Re;
    }
    friend istream& operator>>(istream &in, Complex &cmp) {
        cin>>cmp.Re>>cmp.Im;
        return in;
    }
    friend ostream& operator<<(ostream &out, Complex cmp) {
        out<<cmp.Re<<' '<<cmp.Im<<endl;
        return out;
    }
       
};

std::vector <Complex> f2s(const std::vector<Complex> &f) {
    int n = f.size();
    std::vector<Complex> c;
    c.resize(n);
    for (int k = 0; k < n; k++) {
        for (int j = 0; j < n; ++j) 
            c[k] = c[k] + f[j] ^ (((-2) * M_PI * k * j) / n);
        c[k] = c[k] / n;
    }
    return c;
}
std::vector <Complex> s2f(const std::vector<Complex> &c) {
    int n = c.size();
    std::vector<Complex> f;
    f.resize(n);
    for (int j = 0; j < n; j++)
        for (int k = 0; k < n; k++)
            f[j] = f[j] + c[k] ^ ((2 * M_PI * k * j) / n);
    return f;
}
std::vector<Complex> fft(std::vector<Complex> a, int n, bool switch_fft) {
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
            
 
int main() {
    std::ifstream input("07070057.wav", std::ios_base::binary);
    std::istreambuf_iterator<char> start(input);
    std::istreambuf_iterator<char> end;
    std::vector<char> buf(start, end);
//    cout<<buf.size()<<endl;
    vector<Complex> comp = parse(buf, 0);
    std::ofstream file;
    file.open("f.txt");
    int size = 4410, shift = 5000;
    vector<Complex> vec(comp.begin() + shift, comp.begin() + shift + size);
    for (int i = 0; i < size; i++) {
        file<<i + shift<<' '<<comp[i]<<endl;
    }
    file.close();

/*
    vector<Complex> comp1 = parse(buf, 1);
    while (comp.size() != 1048576) { //здесь до ближайшей степени двойки.Я тут просто сама вписала значение
        comp.push_back(Complex(0, 0));
        comp1.push_back(Complex(0, 0));
    }
    std::vector<Complex> spectr2 = fft(comp, comp.size(), 0);
    std::vector<Complex> spectr3 = fft(comp1, comp1.size(), 0);
//----TASK 1----
//obertone    
//переменные "time" и "p" подаются на входе
//и сам звуковой файл тоже будет подаваться на вход
    double time = 0.01; //!
    int p = 0.5;  //!
    double max = 0.0;
    int prev = 0, step = 44100 * time, count = 0; 
//    cout<<step<<endl;
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

//----TASK 2----
//change spectr

    vector<Complex> vec(spectr2.size() / 2);
    vector<Complex> vec1(spectr3.size() / 2);
    int n = spectr2.size();
    for (int i = 0; i < n / 2; i++) {
        vec[i] = spectr2[(int)(i * 1.5)];
//        vec1[i] = spectr3[(int)(i * 1.5)];
        vec[n - 1 - i] = spectr2[(n - 1 - i) * 1.5];
    }
    vector<Complex> rev = fft(vec, vec.size(), 1); 
    vector<Complex> rev1 = fft(vec1, vec1.size(), 1); 
    write_to_chanel(rev, rev1, spectr2.size(), buf); 
*/

    return 0;
}
