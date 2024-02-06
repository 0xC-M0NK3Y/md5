# C implementation of md5

## Overview

Its a simple C md5 implementation,  
It comes with a library, a binary, and a tester  
  
to compile it do  
```sh
  $ make
```
  
to use the library use the md5.a in the lib folder  
its easy to use, like every implementation there is three functions (look md5.h):  
  
```c
void md5_init(md5_t *ctx);
void md5_update(md5_t *ctx, const uint8_t *input, size_t input_size);
void md5_finish(md5_t *ctx, uint8_t hash[MD5_SIZE]);
```
  
the binary md5 in the bin folder take excactly one argument and computes the hash and prints it  
you can compare it to other md5 implementations  
  
```sh
  $ ./bin/md5 "0xC_M0NK3Y"
  95f3cf81970039860adba9e9d35a0b31
  $ echo -n "0xC_M0NK3Y" | md5sum
  95f3cf81970039860adba9e9d35a0b31  -
```
  
you can also test my implementation with the tester.sh script  
take a look at it, you can change the number of tests and the max length of the strings tested  
it perfoms a diff on the results of my md5 and md5sum  
  
you can also compile in debug mode, it prints some informations while hashing  
  
```sh
  $ make debug
```
  
you will get the debug version of the library and binary
