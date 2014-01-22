#!/bin/bash

# use the following input file:
# https://cvs.khronos.org/svn/repos/ogl/trunk/doc/registry/public/api/gl.xml

if [ "$#" -lt 1 ]; then
	echo -e "usage: $0 INPUTFILE [OUPUTFILE]"
	exit
fi

INPUT=$1

if [ "$#" -ge 2 ]; then
	OUTPUT=$2
else
	OUTPUT="output.h"
fi

PRE="#include <unordered_map>\n\
#include <string>\n\
#include <glow/Extension.h>\n\
\n\
std::unordered_map<std::string, glow::Extension> GLextensions = {
"
POST="};"

echo -e $PRE > $OUTPUT
cat $1 | grep "<extension " | grep -oE "name=\"(.*)\"" | grep -oE "GL_[0-9A-Za-z_]*" | cut -c 4- | sort | uniq | xargs -I$ echo -e "\t{ \"GL_$\", glow::GLOW_$ }," >> $OUTPUT
echo -e $POST >> $OUTPUT
