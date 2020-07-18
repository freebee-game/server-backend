# freebee Makefile

CC ?=		cc
CFLAGS ?=	-O2 -pipe

# Remove if your system doesn't have
# arc4random_uniform(3) and/or strlcat(3)
# Same with pledge(2)
CFLAGS +=	-DHAVE_ARC4RANDOM_UNIFORM -DHAVE_STRLCAT
CFLAGS +=	-DHAVE_PLEDGE

PROG1 =	freebee-daily
PROG2 =	freebee-random
PROG3 =	freebee-yesterday
OBJS =	arc4random_uniform.o main.o strlcat.o
OBJS1 =	anagram-daily.o
OBJS2 =	anagram-random.o
OBJS3 =	anagram-yesterday.o

all: ${OBJS} ${OBJS1} ${OBJS2} ${OBJS3}
	${CC} -static ${LDFLAGS} -o ${PROG1} ${OBJS1} ${OBJS}
	${CC} -static ${LDFLAGS} -o ${PROG2} ${OBJS2} ${OBJS}
	${CC} -static ${LDFLAGS} -o ${PROG3} ${OBJS3} ${OBJS}

# This works for OpenBSD.
# Tweak for your system as needed.
install:
	install -c -s -o www -g www -m 500 ${PROG1} \
		${PROG2} ${PROG3} /var/www/cgi-bin

clean:
	rm -f ${PROG1} ${PROG2} ${PROG3} ${OBJS1} ${OBJS2} \
		${OBJS3} ${OBJS} ${PROG1}.core \
			${PROG2}.core ${PROG3}.core
