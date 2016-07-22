PREFIX ?= ..

export GBDKDIR=${PREFIX}/gbdk/

EMS-FLASHER_DIR ?= ${PREFIX}/ems-flasher

CC	= ${GBDKDIR}bin/lcc -Wa-l -Wl-m -Wl-j

BIN	= PprzOnGB.gb

all:	$(BIN)

upload: $(BIN)
	${EMS-FLASHER_DIR}/ems-flasher --write ${BIN}

delete:
	${EMS-FLASHER_DIR}/ems-flasher --delete 2

make.bat: Makefile
	echo "REM Automatically generated from Makefile" > make.bat
	make -sn | sed y/\\//\\\\/ | grep -v make >> make.bat

%.o:	%.c
	$(CC) -c -o $@ $<

%.s:	%.c
	$(CC) -S -o $@ $<

%.o:	%.s
	$(CC) -c -o $@ $<

%.gb:	%.o
	$(CC) -o $@ $<

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm

