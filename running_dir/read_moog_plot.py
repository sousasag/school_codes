#!/usr/bin/python

import argparse
import re
import numpy as np
from numpy import linalg
import matplotlib.pyplot as plt
try:
    import seaborn
except ImportError:
    pass


def read_moog(filenamemoog, linesfe1, linesfe2):
    try:
        filemoog = open(filenamemoog, 'r')
    except IOError:
        raise IOError('ERROR: File (%s) not present?' % filenamemoog)
#    print 'Reading the file: ' + filenamemoog
    flagfe = 0
    slope_ep = 0
    slope_rw = 0
    diff_feh = 0
    feh1 = 0
    feh2 = 0
    for line in filemoog:
        line = line.strip()
        m = re.search(r'.*Teff= (\d*)\s*log g= (\d*\.\d*)\s*vt= (\d*\.\d*)\s*M/H=\s*([-\d]\d*\.\d*).*', line)
#        m = re.search(r'.*Teff= (\d*)\s*log g= (\d*\.\d*)\s*vt= (\d*\.\d*)\s*\[M/H\]=\s*([-\d]\d*\.\d*).*', line)
        if m:
            teff = m.group(1)
            logg = m.group(2)
            vt = m.group(3)
            feh = m.group(4)
            print "Model Parameters: Teff logg vtur [M/H]"
            print "                  ", teff, logg, vt, feh
        m = re.search(r'Abundance Results for Species (Fe I\s)\.*', line)
        if m:
            flagfe = 1
        m = re.search(r'Abundance Results for Species (Fe II)\.*', line)
        if m:
            flagfe = 2

        m = re.search(r'[a-z]', line)
        if m is None:
            m = re.search(r'[\d]', line)
            if m:
                if flagfe == 1:
                    linesfe1.append(line)
                if flagfe == 2:
                    linesfe2.append(line)

        m = re.search(r'E.P. correlation', line)
        if m:
            if flagfe == 1:
                lines = line.split()
                slope_ep = lines[4]
        m = re.search(r'R.W. correlation', line)
        if m:
            if flagfe == 1:
                lines = line.split()
                slope_rw = lines[4]

        m = re.search(r'average abundance', line)
        if m:
            if flagfe == 1:
                lines = line.split()
                feh1 = float(lines[3])
            if flagfe == 2:
                lines = line.split()
                feh2 = float(lines[3])

    diff_feh = feh1-feh2
    par = teff, logg, vt, feh
    return (slope_ep, slope_rw, diff_feh, feh1, par)


def linear_fit(x, y):
    A = np.array([x, np.ones(len(x))])
    w = linalg.lstsq(A.T, y)[0]  # obtaining the parameters
    xline = np.arange(min(x), max(x), (max(x)-min(x))/100)
    line = w[0]*xline+w[1]  # regression line
    return (w, xline, line)


def plot_graphs(linesfe1, linesfe2, par):
    (teff, logg, vt, feh) = par
    ep = [float(line.split()[2]) for line in linesfe1]
    rw = [float(line.split()[5]) for line in linesfe1]
    ab = [float(line.split()[6]) for line in linesfe1]
    ab2 = [float(line.split()[6]) for line in linesfe2]
    fig = plt.figure()
    ax1 = fig.add_subplot(2, 1, 1)
    ax2 = fig.add_subplot(2, 1, 2)
    difffeh = np.mean(ab)-np.mean(ab2)
    stringin = 'FeI - FeII: %.3f' % difffeh
    ax1.plot(ep[0], ab[0], marker="o", color='b', label=stringin)
    for i in range(1, len(ep)):
        ax1.plot(ep[i], ab[i], marker="o", color='b')
    # ax1.set_ylim([7.02, 8.52])
    ax1.set_xlim(0, max(ep))
    ax1.set_xlabel("E.P.")
    ax1.set_ylabel("Ab FeI")
    # plotting the line

    (w, xline, line) = linear_fit(ep, ab)
    stringin = 'Slope: %.3f' % w[0]
    ax1.plot(xline, line, linestyle='--', color='r', label=stringin)
    stringin = '<[Fe/H]>: %.3f' % (np.mean(ab)-7.47)
    ax2.plot(rw[0], ab[0], marker="o", color='b', label=stringin)
    for i in range(len(rw)):
        ax2.plot(rw[i], ab[i], marker="o", color='b')

    # ax2.set_ylim([7.02, 8.52])
    ax2.set_xlim(min(rw), max(rw))
    ax2.set_xlabel("R.W.")
    ax2.set_ylabel("Ab FeI")
    # plotting the line
    (w, xline, line) = linear_fit(rw, ab)
    stringin = 'Slope: %.3f' % w[0]
    ax2.plot(xline, line, linestyle='--', color='r', label=stringin)
    ax1.legend(frameon=False, fontsize=14)
    ax2.legend(frameon=False, fontsize=14)

    ax1.text(0.05, 0.85, r'Teff: %s  log g: %s  [Fe/H]: %s  $v_{tur}$: %s' % (teff, logg, feh, vt),
        verticalalignment='bottom', horizontalalignment='left',
        transform=ax1.transAxes,
        color='black', fontsize=14)
    fig.tight_layout()
    plt.show()


# Main program:
def main():
    parser = argparse.ArgumentParser(description='Plot the MOOG output')
    parser.add_argument('fname', help='MOOG output file')
    parser.add_argument('-p', '--plot', default=True, action='store_false', help='Plot the results')
    args = parser.parse_args()

    filenamemoog = args.fname
    flagplot = args.plot
    linesfe1 = []
    linesfe2 = []
    (slope_ep, slope_rw, diff_feh, feh1, par) = read_moog(filenamemoog, linesfe1, linesfe2)
    (teff, logg, vt, feh) = par
    print '-----------------------------'
    print '|   Slope  E.P. :'+slope_ep
    print '|   Slope  R.W. :'+slope_rw
    print '|  Fe I  - Fe II:'+str(diff_feh)
    print '|      [Fe/H]   :'+str(feh1-7.47)
    print '| [FE/H] - [M/H]:'+str(feh1-7.47-float(feh))
    print '-----------------------------'
    if flagplot:
        plot_graphs(linesfe1, linesfe2, par)


if __name__ == "__main__":
    main()
