clean:
	rm -f test
	rm -f plaintest
	rm -f *.o
	rm -f *.a

build:
	gcc -c tst_sfxinsert.c tst_sfxscan.c tst_sfxutilities0.c -Wall -Werror
	ar cr liblist.a tst_sfxinsert.o tst_sfxscan.o tst_sfxutilities0.o
	gcc -o test tst_main.c liblist.a -Wall -Werror
	gcc -o plaintest plaintest.c -Wall -Werror

sfx1k:
	cat SynthData1K.txt | ./test 1000
sfx10k:
	cat SynthData10K.txt | ./test 10000
sfx20k:
	cat SynthData20K.txt | ./test 20000
sfx30k:
	cat SynthData30K.txt | ./test 30000
sfx40k:
	cat SynthData40K.txt | ./test 40000
sfx50k:
	cat SynthData50K.txt | ./test 50000
sfx60k:
	cat SynthData60K.txt | ./test 60000
sfx70k:
	cat SynthData70K.txt | ./test 70000
baselines:
	./plaintest

