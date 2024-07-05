###################################################
#
# (C) 2024 Zoff
#
# tox ngc kick bot
#
###################################################

MINGWLIBS=

ismingw = 0
ccmachine = $(shell $(CC) -dumpmachine)
ifeq ($(findstring mingw, $(ccmachine)), mingw)
  ismingw = 1
  CFLAGS += --static
  MINGWLIBS=-liphlpapi
  CFLAGSSQLITE3 += --static
endif

CFLAGS += -g -O3 -fPIC -I./
LIBS = $(shell pkg-config --cflags --libs libsodium) -pthread
TP_CFLAGS = -Wall -Wextra -std=c99 -pedantic
# -Wno-unused-parameter
LIBSSQLITE3 = -lm
CFLAGSSQLITE3 += -g -O3 -fPIC -I./ -Wall -Wextra -Werror=maybe-uninitialized -Wdouble-promotion -Wno-unknown-warning-option -std=c99 -pedantic -pthread

tox_ngckick_bot: tox_ngckick_bot.c toxcore_amalgamation.a sql_tables/gen/csorma.a sqlite3.a
	$(CC) $(CFLAGS) $(TP_CFLAGS) -Wno-pointer-sign -Wno-unused-function -Wno-unused-parameter tox_ngckick_bot.c toxcore_amalgamation.a sql_tables/gen/csorma.a sqlite3.a $(LIBS) $(MINGWLIBS) -o tox_ngckick_bot

sqlite3.a: sqlite3.o
	$(AR) rcs sqlite3.a sqlite3.o

sqlite3.o: csorma/sqlite/sqlite3.c
	$(CC) -c $(CFLAGSSQLITE3) $(LIBSSQLITE3) -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-pointer-arith -Wno-unused-variable -Wno-sign-compare -Wno-double-promotion $< -o $@

sql_tables/gen/csorma.a: sql_tables/*.java
	cd csorma/;javac csorma_generator.java
	cd csorma/;java csorma_generator ../sql_tables/
	cd ./sql_tables/gen/; $(MAKE) csorma.a

toxcore_amalgamation.a: toxcore_amalgamation.o
	ar rcs toxcore_amalgamation.a toxcore_amalgamation.o

toxcore_amalgamation.o: toxcore_amalgamation_no_toxav.c
	$(CC) -c -DTOX_CAPABILITIES_ACTIVE $(CFLAGS) $(LIBS) $< -o $@

clean2:
	rm -f tox_ngckick_bot sql_tables/gen/csorma.a sql_tables/gen/Makefile sql_tables/gen/*.* sql_tables/gen/sqlite/*

clean:
	rm -f ToxProxy toxcore_amalgamation.o toxcore_amalgamation.a sql_tables/gen/csorma.a sql_tables/gen/Makefile sql_tables/gen/*.* sql_tables/gen/sqlite/* sqlite3.a sqlite3.o

