CFLAGS=-W -Wall -DLINUX -g
OBJECTS=mem.o tshtable.o tsllist.o

includeCrawler: includeCrawler.o $(OBJECTS)
	gcc -g -o includeCrawler includeCrawler.o $(OBJECTS) -lpthread

includeCrawler.o: includeCrawler.c tshtable.h tsllist.h mem.h
mem.o: mem.c mem.h
tshtable.o: tshtable.c tshtable.h mem.h
tsllist.o: tsllist.c tsllist.h mem.h

clean:
	rm -f *.o includeCrawler *~
