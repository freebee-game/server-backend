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
OBJS =	arc4random_uniform.o main.o strlcat.o
OBJS1 =	anagram-daily.o
OBJS2 =	anagram-random.o

all: ${OBJS} ${OBJS1} ${OBJS2}
	${CC} -static ${LDFLAGS} -o ${PROG1} ${OBJS1} ${OBJS}
	${CC} -static ${LDFLAGS} -o ${PROG2} ${OBJS2} ${OBJS}

clean:
	rm -f ${PROG1} ${PROG2} ${OBJS1} ${OBJS2} ${OBJS} \
		${PROG1}.core ${PROG2}.core
