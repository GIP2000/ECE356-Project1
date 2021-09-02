all: hash.o spellcheck.o spell.out

spell.out: spellcheck.o hash.o
	g++ -o spell.out spellcheck.o hash.o

spellcheck.o: spellcheck.cpp hash.h
	g++ -c spellcheck.cpp

hash.o: hash.cpp hash.h
	g++ -c hash.cpp

debug:
	g++ -g -o spellDebug.out spellcheck.cpp hash.cpp

clean:
	rm -f *.out *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
