# ARES
With ARES you can automatically measure the EW from a spectrum.

Run `ARES` in the `running_dir` directory after editing the `mine.opt` file:

   * specfits: Use the right PATH (spectra in the `spectra` folder)
   * fileout: Call it `<spectrum>.ares`
   * rejt: For high SNR this should be close to 1, otherwise it should get
   closer to 0. rejt=1-1/SNR
   * plots_flag: 1: See plots of the fitted profile, 0 otherwise.

# TMCalc
TMCalc gives a fast estimate of the effective temperature and metallicity
based on the output from ARES.

In the TMCALC folder, run:

    ./TMCalc.bash ../running_dir/<spectrum>.ares

Note down the temperature and metallicity, since this will be used in the next
step




# Line list for MOOG
MOOG needs an atomic line list in a very specific format. We have a script
to combine the output from ARES (the EWs) with the other atomic data.
In the root directory run

    python make_moog_lines.py running_dir/<spectrum>.ares ironlines_parameters.dat

This will create a file: `running_dir/lines.<spectrum>.ares`
We are now ready to derive parameters.







# Getting the atmospheric parameters
For this task go to `running_dir`. Here you change the line `lines_in`
in `batch.par` to use the line list for the star you are interested in.

Now run

    python interpol_MOOG.py teff logg [Fe/H] vt

where `teff` is the effective temperature, `logg` is the surface gravity,
`[Fe/H]` is the metallicity, and `vt` is the microturbulence (0-9.99).
Example

    python interpol_MOOG.py 5777 4.44 0.00 1.00  # This is solar values

If the user do not want the plot, simply add a `-p` flag:


    python interpol_MOOG.py 5777 4.44 0.00 1.00 -p
    # or
    python interpol_MOOG.py -p 5777 4.44 0.00 1.00

Get the slopes as close to 0 as possible, and the difference between
the FeI and FeII abundance should be 0 too. Last, the output metallicity (we
use `[Fe/H]` as a proxy) should match the input metallicity.
