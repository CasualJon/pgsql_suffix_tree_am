***README.txt***

Prefixed-Suffix Trie Project, CS-764, UW-Madison, Fall 2018
  James Goodman
  Jon Cyrus


Makefile commands:
<make clean>
clean - removes all executable, object, and library files (.out, .o, .a)

<make build>
build - compiles 2 c programs
    1 plaintest.c (used to build baselines for sequential scan equivalents):
        All code contained within plaintest.c (though it references txt files)
        Line 7 defines NUMOFDATA to build the mock-up of the synthetic data
        Line 30 contains the file name necessary to load the correct dataset

    2 tst_main.c (used to drive the connstruction and testing of the trie):
        tst_main.c is the driver for the core files: tst_sfxinsert.c,
        tst_sfxscan.c, tst_sfxutilities0.c, and tst_hdr.h.
        For space restriction, MAX_CHAR is limited to 27 (NUL and a-z in ASCII),
        and MAX_VAL is limited to 100 (lines 17 and 20, tst_hdr.h) in order to
        constrain the size of the arrays built within this implementation

<make ...>
sfx1k - builds the trie with 1,000 tuples (0:999) and executes tests
sfx10k - builds the trie with 10,000 tuples(0:9,999) and executes tests
sfx20k - builds the trie with 20,000 tuples(0:19,999) and executes tests
sfx30k - builds the trie with 30,000 tuples(0:29,999) and executes tests
sfx40k - builds the trie with 40,000 tuples(0:39,999) and executes tests
sfx50k - builds the trie with 50,000 tuples(0:49,999) and executes tests
sfx60k - builds the trie with 60,000 tuples(0:59,999) and executes tests
sfx70k - builds the trie with 70,000 tuples(0:69,999) and executes tests
baselines - builds a linked list representation of tuples and sequentially scans
    each using different string comparison tools
