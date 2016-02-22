CFLAGS = -std=c99 -Wall -Wno-unused-variable -Wno-deprecated-declarations -DMAX_ITERS=100 -DNTH=8 -DOSX -DBUF_SZ=1048576 -DVERBOSE_MT=0 -I/opt/AMDAPPSDK-3.0/include -L/opt/AMDAPPSDK-3.0/lib/x86_64
# If you have a GPU, add this to your CFLAGS: -DGPU

CC = gcc
ALL = testbench

all: $(ALL)


testbench: testbench.o driver.o
	$(CC) -L/opt/AMDAPPSDK-3.0/lib/x86_64 testbench.o driver.o -o testbench -lpthread -lOpenCL


testbench.o: testbench.c
	$(CC) $(CFLAGS) -pthread -c testbench.c -lOpenCL

driver.o: driver.c
	$(CC) $(CFLAGS) -c driver.c

clean:
	rm *.o testbench
