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
				
		self.requirements = []
		for req in xml.findall("require"):
			for child in req:
				if child.tag in ("enum", "command"):
					self.requirements.append(child.attrib["name"])
		
	def __str__(self):
		return "Extension(%s)" % (self.name)
	
features = []
extensions = []

for feature in registry.iter("feature"):
	features.append(Feature(feature))
	
#features = filter(lambda f: f.prefix=="GL_VERSION", features)
	
for extensionGroup in registry.iter("extensions"):
	for extension in registry.iter("extension"):
		extensions.append(Extension(extension))
	
featureMap = dict()
deprecatedMap = dict()

for f in features:
	for req in f.requirements:
		if not req in featureMap or f<featureMap[req]:
			featureMap[req] = f
	for dep in f.deprecates:
		if not dep in deprecatedMap or f<deprecatedMap[dep]:
			deprecatedMap[dep] = f
		
"""
for r in sorted(featureMap.keys()):
	print(r+":")
	print("\tsince: "+str(featureMap[r]))
	if r in deprecatedMap:
		print("\tdepracated: "+str(deprecatedMap[r]))
	
"""	
	
foo = dict()

for e in extensions:
	required = set()
	for r in e.requirements:
		if r in featureMap:
			required.add(str(featureMap[r]))
		else:
			#print("\t%s not in map" % r)
			required.add("---")
	a = sorted(required)
	if len(a)>1 and not "---" in a:
		print("%s:" % e)
		print("\t%s" % a)
		
		