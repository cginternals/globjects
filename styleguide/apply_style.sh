#!/bin/bash
cd `dirname $0`
cd ..

find . \( -ipath "*source*" -or -ipath "*include*" -or -ipath "*example*" \) -and \( -iname "*.h" -or -iname "*.hpp" -or -iname "*.cpp" \) | xargs -I{} uncrustify -c styleguide/uncrustify.cfg --no-backup -l CPP {}
