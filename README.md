# Azores 2016 summer school
## Asteroseismology and Exoplanets: Listening to the Stars and Searching for New Worlds

Some useful codes for teaching spectroscopic stellar parameters





##Contents of this repository

  * [ARES](https://github.com/sousasag/ARES): A code to automatically measure Equivalent Widths of absorption lines is stellar spectra.
  * [TMCALC](https://github.com/sousasag/TMCALC): A code that reads ARES output and computes a fast estimation of Teff and [Fe/H].
  * [MOOG2014](http://www.as.utexas.edu/~chris/moog.html): A code that performs a variety of LTE line analysis and spectrum synthesis tasks. (This version was modified so it does not uses supermongo plots).
  * [interpol_models_marcs](https://github.com/sousasag/school_codes/tree/master/interpol_models_marcs): A tool to automatically interpolate [MARCS models](http://marcs.astro.uu.se/) using the respective interpolation [code](http://marcs.astro.uu.se/software.php).
  * [make_moog_lines.py](https://github.com/sousasag/school_codes/blob/master/make_moog_lines.py): A python code that will transform ARES output format into MOOG input linelist
  * [read_moog_plot.py](https://github.com/sousasag/school_codes/blob/master/read_moog_plot.py): A python code that will read the output from MOOG and make some plots and output some results important for the spectroscopic parameter determination.
  * [ironlines_parameters.dat](https://github.com/sousasag/school_codes/blob/master/ironlines_parameters.dat): A linelist of FeI and FeII that was compiled for precise determination of spectroscopic parameters [Sousa et al. 2008](http://adsabs.harvard.edu/abs/2008A%26A...487..373S).
  * [ultimate_list.dat](http://adsabs.harvard.edu/abs/2008A%26A...487..373S): A linelist optimized for the use of TMCALC.
  * [spectra](https://github.com/sousasag/school_codes/tree/master/spectra): A folder wich contains some anonymous test HARPS spectra for the exercises in the class.


## Installation

Simply clone the repository:

```
git clone --recursive https://github.com/sousasag/school_codes
```

To make the interpolation of the codes you need the grid of MARCS models with
can be downloaded and extracted by:

```
cd school_codes
wget www.astro.up.pt/~sousasag/xpto123/marcs_models.tar
mv marcs_models.tar interpol_models_marcs
cd interpol_models_marcs
tar xvf marcs_models.tar
rm marcs_models.tar
cd ..
```

Before compiling the codes, make sure to edit line 22 of
`MOOG2014/Moogsilent.f`, so it match your PATH.

Compile the codes from the root directory:

```
make
```

If you need to clean the compilation:

```
make clean
```


##Other useful codes to install:

[IRAF](http://iraf.noao.edu/). A guide for installation at unix can be found [here](http://www.astronomy.ohio-state.edu/~khan/iraf/iraf_step_by_step_installation).

There are several ways to install IRAF on your system. One that has been easy to
use recently and is recommended is with [Ureka](http://ssb.stsci.edu/ureka/)

## Requirements

These codes were implemented to be used on a Linux machines. Here is the list of
some of the requirements that you need to install these codes in order to make the
codes running:

For Python
  * re
  * numpy
  * matplotlib
  * argparse
  * seaborn (not necessary, but will make the plots look slightly better)

They can all be installed with `pip install numpy matplotlib argparse seaborn`


For ARES:
  * [CFITSIO](http://heasarc.nasa.gov/fitsio/fitsio.html)
  * [GNU Compiler Collection](http://gcc.gnu.org/)
  * [GNU Scientific Library](http://www.gnu.org/software/gsl/)
  * [Open Multi-Processing library](http://openmp.org/wp/)
  * [The plotutils Package](http://www.gnu.org/software/plotutils/)
  * [gnuplot library](http://www.gnuplot.info/)

All of these are easily installed in many Linux flavour systems with the use of
the respective repositories. As an example for Ubuntu:
```
sudo apt-get install libcfitsio3-dev gcc libgsl0-dev plotutils gnuplot g++ csh
```

For TMCALC you need to also have `cython` installed if you want to create a
python module to interface with TMCALC (`pip install cython`).

