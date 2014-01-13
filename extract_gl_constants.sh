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

PRE="#include <GL/glew.h>\n\
#include <unordered_map>\n\
\n\
std::unordered_multimap<GLuint64, const char*> GLconstants = {
"
POST="};"

echo -e $PRE > $OUTPUT
cat $1 | grep "<enum " | grep -oE "name=\"(.*)\"" | grep -oE "GL_[0-9A-Za-z_]*" | sort | uniq | xargs -I$ echo -e "#ifdef $\n\t{ $, \"$\" },\n#endif" >> $OUTPUT
echo -e $POST >> $OUTPUT
