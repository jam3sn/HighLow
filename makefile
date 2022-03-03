all:
	/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -c -o main.o src/main.c
	/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -o HighLow.gb main.o
