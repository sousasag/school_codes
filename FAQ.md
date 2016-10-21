Things that can give problems for TE2...

ARES:

Problem:
 * Missing PATH for GSL/CFITSIO/etc.
  
Solution:  
Find where the libraries are installed and include them
 * gcc .... -L/path/to/missing/lib/ -I/path/to/missing/include

Problem:
 * Problems compiling with GSL v. 2.x
 
Solution:
 * mv areslib_gsl2.h areslib.h
 * make

TMCALC:

Problem:
  * Invalid command code T
  
Solution:  
Change the PATH in TMCalc.bash to the right PATH
 * gcc -g -ansi -Wall -c -lm
 * cd tmcalc_cython; python setup.py build_ext --inplace
