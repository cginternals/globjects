#!/usr/bin/python

import sys, getopt
import xml.etree.ElementTree as ET

inputfile = "gl.xml"
outputfile = "Extension.h"
outputfile2 = "gl_extension_info.h"

try:
	opts, args = getopt.getopt(sys.argv[1:], "i:o:o2:", ["input=","output=", "output2="])
except getopt.GetoptError:
	print("usage: %s [-i <input>] [-o <output>] [-o2 <output2>]" % sys.argv[0])
	sys.exit(1)
	
for opt, arg in opts:
	if opt in ("-i", "--input"):
		inputfile = arg
	elif opt in ("-o", "--output"):
		outputfile = arg
	elif opt in ("-o2", "--output2"):
		outputfile2 = arg


tree = ET.parse(inputfile)
registry = tree.getroot()

class Feature:
	def __init__(self, xml):
		self.name = xml.attrib["name"]
		
		self.prefix = self.name[:-4]
		self.major = self.name[-3:-2]
		self.minor = self.name[-1:]
		
		self.requirements = []
		for req in xml.findall("require"):
			for child in req:
				if child.tag in ("enum", "command"):
					self.requirements.append(child.attrib["name"])
					
		self.deprecates = []
		for rem in xml.findall("remove"):
			for child in rem:
				if child.tag in ("enum", "command"):
					self.deprecates.append(child.attrib["name"])
		
	def __str__(self):
		return "Feature(%s:%s.%s)" % (self.prefix, self.major, self.minor)
		
	def __lt__(self, other):
		return self.major<other.major or (self.major==other.major and self.minor<other.minor)
	
class Extension:
	def __init__(self, xml):
		self.name = xml.attrib["name"]
		self.incore = None
		
		self.requirements = []
		for req in xml.findall("require"):
			for child in req:
				if child.tag in ("enum", "command"):
					self.requirements.append(child.attrib["name"])
		
	def __str__(self):
		return "Extension(%s)" % (self.name)
		
	def enumName(self):
		return "GLOW_"+ self.name[3:]
	
features = []
extensions = []

for feature in registry.iter("feature"):
	features.append(Feature(feature))
	
features = filter(lambda f: f.prefix=="GL_VERSION", features)
	
for extensionGroup in registry.iter("extensions"):
	for extension in registry.iter("extension"):
		extensions.append(Extension(extension))
	
featureMap = dict()
deprecatedMap = dict()

for f in features:
	for req in f.requirements:
		if not req in featureMap:
			featureMap[req] = set()
		featureMap[req].add(f)
	for dep in f.deprecates:
		if not dep in deprecatedMap:
			deprecatedMap[dep] = set()
		deprecatedMap[dep].add(f)
		
"""
for r, fs in featureMap.items():
	if len(fs)==1 and any(ff.prefix != "GL_VERSION" for ff in fs):
		s = set()
		for f in fs:
			s.add(str(f))
		print("%s: %s" % (r,list(s)))

sys.exit(0)
"""

for e in extensions:
	required = dict()
	deprecated = dict()
	for r in e.requirements:
		required[r] = featureMap.get(r, None)
		if r in deprecatedMap:
			deprecated[r] = deprecatedMap[r]
		
	if len(required)>0 and not any(v is None for v in required.values()):
		#print("%s:" % e)
		s = set()
		for fs in required.values():
			for f in fs:
				s.add(f) 
		#print("\tsince: %s" % max(s))
		e.incore = max(s)
		"""
		if len(deprecated)>0:
			d = set()
			for k,v in deprecated.items():
				d.add("%s -> %s" % (k,min(v)))
			print("\tdeprecated: %s" % list(d))
		"""

			
enumDecl = "enum Extension\n{\n"
namesMap = "std::unordered_map<glow::Extension, std::string> extensionStrings = {\n"
extensionMap = "std::unordered_map<std::string, glow::Extension> extensions = {\n"
incoreMap = "std::unordered_map<glow::Extension, glow::Version> extensionVersions = {\n"

for e in extensions:
	enumDecl += "\t%s,\n" % e.enumName()
	namesMap += '\t{ glow::%s, "%s" },\n' % (e.enumName(), e.name)
	extensionMap += '\t{ "%s", glow::%s },\n' % (e.name, e.enumName())
	if e.incore:
		incoreMap += "\t{ glow::%s, glow::Version(%s, %s) },\n" % (e.enumName(), e.incore.major, e.incore.minor)
	
enumDecl += "\tGLOW_Unknown_Extension\n}"
namesMap += "};\n"
extensionMap += "};\n"
incoreMap += "};\n"

		
pre = """#pragma once

namespace glow {

"""
post = """

} // namespace glow
"""

with open(outputfile, 'w') as file:
	file.write(pre)
	file.write(enumDecl)
	file.write(post)
	
pre = """#include <string>
#include <unordered_map>

#include <glow/Extension.h>
#include <glow/Version.h>

"""
post = ""
		
with open(outputfile2, 'w') as file:
	file.write(pre)
	file.write(namesMap)
	file.write("\n")
	file.write(extensionMap)
	file.write("\n")
	file.write(incoreMap)
	file.write(post)
		
