#On Expanse, need to load gcc. For example. module load gcc/7.2.0. 
#On CSIL, this Makefile wors as it is
CC=gcc
CFLAGS= -mavx2 -O
CFLAGS3= -mavx2 -O3
CPPFLAGS=
CFILES= simd_sum.c simd_test.c minunit.c
TARGET=simd_test 
TARGET3=simd_test3

#gcc minunit.c avx_test.c -O -o avx_test -mavx2 -O3

all: $(TARGET) $(TARGET3) 


$(TARGET): $(CFILES)
	$(CC)  $(CFLAGS)  $(CFILES) -o $@

$(TARGET3): $(CFILES)
	$(CC)  $(CFLAGS3) $(CFILES) -o $@
	
run: 
	./$(TARGET)

run3: 
	./$(TARGET3)

clean:
	rm $(TARGET) $(TARGET3)
