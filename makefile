CFLAGS = -Iinclude
CC = gcc

all: p1.x p2.x p3.x

p1.x: part1.o
	$(CC) -g -o $@ $^ 

p2.x: part2.o
	$(CC) -g -o $@ $^ 
	
p3.x: part2.o
	$(CC) -g -o $@ $^ 
	
part1.o: part1-2.c
	$(CC) $(CFLAGS) -D PROB1 -g -c -o $@ $<  
	
part2.o: part1-2.c
	$(CC) $(CFLAGS) -g -c -o $@ $<
	
part3.o: part3.c
	$(CC) $(CFLAGS) -g -c -o $@ $<

clean:
	rm -f *.x *.o *~

