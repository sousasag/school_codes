#!/usr/bin/python

import numpy as np
import argparse


def read_ares_file(fileares):
    """
    Read the ares new output file into a numpy array
    with the respective names in the columns
    """
    data = np.loadtxt(fileares, dtype={'names': ('lambda_rest', 'ngauss', 'depth', 'fwhm', 'ew', 'ew_er', 'c1', 'c2', 'c3'),
                                       'formats': ('f4', 'f4', 'f4', 'f4', 'f4', 'f4', 'f4', 'f4', 'f4')})
    return data


def read_linelist_file(filelinelist):
    """
    Read the linelist file into a numpy array
    with the respective names in the columns
    This skips header with 2 lines
    """
    data = np.loadtxt(filelinelist, dtype={'names': ('lambda_rest', 'EP', 'loggf', 'ele', 'atom'),
                                           'formats': ('f4', 'f4', 'f4', 'a4', 'f4')},
                      skiprows=2)
    return data


def make_lines_moog_file(filename, filename_out, ares_data, linelist_data, llmin, llmax, ewmin, ewmax):
    """
    Creates the lines.file.ares formated for moog with the atomic data
    """
    fileout = open(filename_out, 'w')
    fileout.write(' '+filename+'\n')
    for datai in ares_data:
        lambda_ares = datai['lambda_rest']
        atomic_data = linelist_data[np.where(abs(lambda_ares-linelist_data['lambda_rest']) < 0.1)]
        ew = datai['ew']
        if len(atomic_data) > 0 and ew < ewmax and ew > ewmin and lambda_ares > llmin and lambda_ares < llmax:
            fileout.write('{: 9.2f}{: 8.1f}{: 12.2f}{: 11.3f}{: 28.1f}\n'.format(float(datai['lambda_rest']), float(atomic_data['atom']), float(atomic_data['EP']), float(atomic_data['loggf']), float(datai['ew'])))

    print 'Saved in: %s' % filename_out


# Main program:
def main():
    parser = argparse.ArgumentParser(description='Convert the ARES output to MOOG format')
    parser.add_argument('ares', help='ARES output file')
    parser.add_argument('linelist', help='Linelist used by ARES')
    args = parser.parse_args()

    filename_ares = args.ares
    filename_linelist = args.linelist
    filename_out = list(filename_ares.rpartition('/'))
    filename_out.insert(2, 'lines.')
    filename_out = ''.join(filename_out)

    ares_data = read_ares_file(filename_ares)
    linelist_data = read_linelist_file(filename_linelist)

    llmin = 4500
    llmax = 9000
    ewmin = 5
    ewmax = 150
    make_lines_moog_file(filename_ares, filename_out, ares_data, linelist_data, llmin, llmax, ewmin, ewmax)


if __name__ == "__main__":
    main()
