FLAG = -O3

all:
	g++ -o init init.cpp ${FLAG}
	g++ -o cal cal.cpp ${FLAG}
	g++ -o print print.cpp ${FLAG}
	g++ -o short short.cpp ${FLAG}