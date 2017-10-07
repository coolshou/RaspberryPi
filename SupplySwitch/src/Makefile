#CROSS_COMPILE=arm-linux-gnueabihf-
CC=$(CROSS_COMPILE)gcc

all: softshut
softshut: clean
	$(CC) softshut.c -l rt -l bcm2835 -std=gnu99 -o softshut

clean:
	rm softshut

install:
	cp softshut /usr/bin/
	cp init.d/softshut /etc/init.d/
	
