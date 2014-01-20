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
		
	#a = sorted(required)
	if len(required)>0 and not any(v is None for v in required.values()):
	#~ if len(required)>0:
		print("%s:" % e)
		"""
		for k,v in required.items():
			s = set()
			if v:
				for f in v:
					s.add(str(f))
			print("\t%s: %s" % (k,list(s)))
		"""
		s = set()
		for fs in required.values():
			for f in fs:
				s.add(f) 
		print("\tsince: %s" % max(s))
		if len(deprecated)>0:
			d = set()
			for k,v in deprecated.items():
				d.add("%s -> %s" % (k,min(v)))
			print("\tdeprecated: %s" % list(d))
	
	#if len(a)>1 and not "---" in a:
	#if 1==1:
	#	print("%s:" % e)
	#	print("\t%s" % a)
		
		