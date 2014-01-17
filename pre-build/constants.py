#!/usr/bin/python

import sys, getopt
import xml.etree.ElementTree as ET

inputfile = "gl.xml"
outputfile = "gl_constants.h"

try:
	opts, args = getopt.getopt(sys.argv[1:], "i:o:", ["input=","output="])
except getopt.GetoptError:
	print("usage: %s [-i <input>] [-o <output>]" % sys.argv[0])
	sys.exit(1)
	
for opt, arg in opts:
	if opt in ("-i", "--input"):
		inputfile = arg
	elif opt in ("-o", "--output"):
		outputfile = arg


tree = ET.parse(inputfile)
registry = tree.getroot()

enumNames = []

for enums in registry.iter("enums"):
	if enums.get("namespace", "") == "GL":
		for enum in enums.findall("enum"):
			if "name" in enum.attrib:
				if ("api" in enum.attrib) and (enum.attrib["api"] != "gl"):
					continue
				enumNames.append(enum.attrib["name"])
				
enumNames = sorted(set(enumNames))
				
pre = """#include <GL/glew.h>
#include <unordered_map>

std::unordered_multimap<GLuint64, const char*> GLconstants = {
"""

post = """}
"""

with open(outputfile, 'w') as file:
	file.write(pre)
	for enumName in enumNames:
		file.write("#ifdef %s\n" % enumName)
		file.write('\t{ %s, "%s" },\n' % (enumName, enumName))
		file.write("#endif\n")
	file.write(post)

