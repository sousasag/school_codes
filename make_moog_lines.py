#!/usr/bin/python

##imports:
import numpy as np
import sys

## My functions:



def read_ares_file(fileares):
  """
  Read the ares new output file into a numpy array
  with the respective names in the columns
  """
  data = np.loadtxt(fileares, dtype={'names': ('lambda_rest', 'ngauss', 'depth', \
    'fwhm', 'ew', 'ew_er','c1','c2','c3' ),'formats': ('f4', 'f4', 'f4', 'f4', 'f4', 'f4', 'f4', 'f4','f4')})
  return data

def read_linelist_file(filelinelist):
  """
  Read the linelist file into a numpy array  
  with the respective names in the columns
  This skips header with 2 lines
  """
  data = np.loadtxt(filelinelist, dtype={'names': ('lambda_rest', 'EP', 'loggf', \
    'ele', 'atom' ),'formats': ('f4', 'f4', 'f4', 'a4','f4')},skiprows=2)
  return data


def make_lines_moog_file(filename, filename_out, ares_data, linelist_data, llmin, llmax, ewmin, ewmax):
  """
  Creates the lines.file.ares formated for moog with the atomic data
  """
  fileout = open(filename_out,'w')
  fileout.write(' '+filename+'\n')
  for datai in ares_data:
    lambda_ares = datai['lambda_rest']
    atomic_data = linelist_data[np.where( abs(lambda_ares-linelist_data['lambda_rest']) < 0.1 )]
    ew = datai['ew']
    if len(atomic_data) > 0 and ew < ewmax and ew > ewmin and lambda_ares > llmin and lambda_ares < llmax:
      #print '{: 9.2f}{: 8.1f}{: 12.2f}{: 11.3f}{: 28.1f}'.format(float(datai['lambda_rest']),float(atomic_data['atom']),float(atomic_data['EP']),float(atomic_data['loggf']),float(datai['ew']))
      fileout.write('{: 9.2f}{: 8.1f}{: 12.2f}{: 11.3f}{: 28.1f}\n'.format(float(datai['lambda_rest']),float(atomic_data['atom']),float(atomic_data['EP']),float(atomic_data['loggf']),float(datai['ew'])))



### Main program:
def main():
#  print 'Number of arguments:', len(sys.argv), 'arguments.'
#  print 'Argument List:', str(sys.argv)
  if len(sys.argv) != 3:
    print 'Only 2 argument is needed'
    print 'The argument should be the name of the outputted MOOG file then 0 for no plot, 1 for plot.'
    print 'Example to show plot: ./read_moog_plot.py moog_output.file 1'
    print 'Example without plot: ./read_moog_plot.py moog_output.file 0'
    exit()

  filename_ares     = sys.argv[1]
  filename_linelist = sys.argv[2]
  filename_out = 'lines.'+filename_ares

  ares_data = read_ares_file(filename_ares)
  linelist_data = read_linelist_file(filename_linelist)

  llmin = 4500
  llmax = 9000
  ewmin = 5
  ewmax = 150
  make_lines_moog_file(filename_ares, filename_out, ares_data, linelist_data, llmin, llmax, ewmin, ewmax)

  

if __name__ == "__main__":
    main()

