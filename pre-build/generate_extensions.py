#!/usr/bin/python

import sys, getopt, os

from classes.extensions_generation import *

def main(argv):
	inputfile = "gl.xml"
	outputfile = "Extension.h"
	outputfile2 = "gl_extension_info.h"

	try:
		opts, args = getopt.getopt(argv[1:], "i:o:o2:", ["input=","output=", "output2="])
	except getopt.GetoptError:
		print("usage: %s [-i <input>] [-o <output>] [-o2 <output2>]" % argv[0])
		sys.exit(1)
		
	for opt, arg in opts:
		if opt in ("-i", "--input"):
			inputfile = arg
		elif opt in ("-o", "--output"):
			outputfile = arg
		elif opt in ("-o2", "--output2"):
			outputfile2 = arg

	outputfile = os.path.join("generated", outputfile)
	outputfile2 = os.path.join("generated", outputfile2)

	generateExtensionFiles(inputfile, outputfile, outputfile2)

if __name__ == "__main__":
	main(sys.argv)