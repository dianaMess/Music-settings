install: obertone spectr
obertone:
	g++ -std=c++11 src/fft.cpp -o fft.o -c
	g++ -std=c++11 obertone.cpp -o obertone.o -c
	g++ -std=c++11 fft.o obertone.o -o obertone
spectr:
	g++ -std=c++11 src/fft.cpp -o fft.o -c
	g++ -std=c++11 spectr.cpp -o spectr.o -c
	g++ -std=c++11 fft.o spectr.o -o spectr
equalizer:
	g++ -std=c++11 src/fft.cpp -o fft.o -c
	g++ -std=c++11 equalizer.cpp -o equalizer.o -c
	g++ -std=c++11 fft.o equalizer.o -o equalizer


