#CROSS_COMPILE=arm-linux-gnueabihf-

CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
INCLUDE +=
CFLAGS = -Wall -o2
CXXFLAGS = -Wall -o2
LIBS = -lwiringPi -lpthread

TARGET = softshut
SRCS = softshut.c
OBJS := ${addsuffix .o, $(basename ${SRCS})}

%.o : %.cpp
	${CXX} -c ${CXXFLAGS} ${INCLUDEDIR} $< -o $@

%.o : %.c
	${CC} -c ${CFLAGS} ${INCLUDEDIR} $< -o $@ ${LIBS}

all: ${OBJS}
	${CXX} -Wl,-soname,${TARGET} -o ${TARGET} ${OBJS} ${LIBS}

clean:
	rm -f ${TARGET} ${OBJS} *.bak *~ core .depend

install:
	cp softshut /usr/bin/

