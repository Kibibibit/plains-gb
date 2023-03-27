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

plains.gbc: plains.o iters.o helpers.o sprites.o tiles.o frame_constants.o maths.o fixed.o bkg_funcs.o game_object.o entity.o player.o
	$(CC) $(CFLAGS) -Wm-yC -o plains.gbc plains.o iters.o helpers.o sprites.o tiles.o frame_constants.o maths.o fixed.o bkg_funcs.o game_object.o entity.o player.o
