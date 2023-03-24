CC	= ${GBDK}/bin/lcc -Wa-l -Wl-m -Wl-j

BINS	= plains.gbc

all:	$(BINS)

%.o:	%.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o:	%.s
	$(CC) $(CFLAGS) -c -o $@ $<

%.s:	%.c
	$(CC) $(CFLAGS) -S -o $@ $<

%.gc:	%.o
	$(CC) -o $@ $<

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm *.noi *.adb *.gbc

plains.gbc: plains.o maths.o bank.o fixed.o sprites.o tiles.o bkg_funcs.o entity.o player.o
	$(CC) $(CFLAGS) -Wm-yC -o plains.gbc plains.o maths.o bank.o fixed.o sprites.o tiles.o bkg_funcs.o entity.o player.o
