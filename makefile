all:
	/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o build/main.o src/main.c
	/opt/gbdk/bin/lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -o build/HighLow.gb build/main.o
	mv build/HighLow.gb HighLow.gb
