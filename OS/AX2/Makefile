CFLAGS = -std=c99 -Wall -Wno-unused-variable -Wno-deprecated-declarations -DMAX_ITERS=100 -DNTH=20 -DOSX -DBUF_SZ=1048576 -DVERBOSE_MT=0 -I/opt/AMDAPPSDK-3.0/include
# If you have a GPU, add this to your CFLAGS: -DGPU

CC = gcc
ALL = testbench

all: $(ALL)


testbench: testbench.o driver.o
	$(CC) testbench.o driver.o -lOpenCL -o testbench -lpthread


testbench.o: testbench.c
	$(CC) $(CFLAGS) -pthread -c testbench.c

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

clean:
	rm *.o testbench
