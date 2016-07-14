#!/usr/bin/env python
# -*- coding: utf8 -*-

# My imports
from __future__ import division
import argparse
import os


def interpolate(params):
    teff, logg, feh, vt = params
    path = '../interpol_models_marcs/./make_model_marcs.bash'
    os.system('%s %i %.2f %.2f %.2f > /dev/null' % (path, teff, logg, feh, vt))


def runMOOG():
    os.system('../MOOG2014/./MOOGSILENT > /dev/null')


def _parser():
    parser = argparse.ArgumentParser(description='Interpolate and run MOOG with new model atmosphere')
    parser.add_argument('teff', type=int, help='Effective temperature')
    parser.add_argument('logg', type=float, help='Surface gravity (logg)')
    parser.add_argument('feh', type=float, help='Metallicity ([Fe/H])')
    parser.add_argument('vt', type=float, help='Microturbulence (km/s)')
    return parser.parse_args()


if __name__ == '__main__':
    args = _parser()

    params = (args.teff, args.logg, args.feh, args.vt)
    interpolate(params)

    runMOOG()

    print "MOOG is done"
    os.system("rm -f a")  # Cleaning unnecessary files
