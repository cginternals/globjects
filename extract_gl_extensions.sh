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

PRE="#pragma once\n\
\n\
namespace glow {\n\
\n\
enum Extension {
"
POST="\tGLOW_Unknown\n\
};\n\
\n\
} //namespace glow"

echo -e $PRE > $OUTPUT
#cat $1 | grep "<extension " | grep -oE "name=\"(.*)\"" | grep -oE "GL_[0-9A-Za-z_]*" | cut -c 4- | sort | uniq | xargs -I$ echo -e "#ifdef GL_$\n\t$,\n#endif" >> $OUTPUT
cat $1 | grep "<extension " | grep -oE "name=\"(.*)\"" | grep -oE "GL_[0-9A-Za-z_]*" | cut -c 4- | sort | uniq | xargs -I$ echo -e "\tGLOW_$," >> $OUTPUT
echo -e $POST >> $OUTPUT
