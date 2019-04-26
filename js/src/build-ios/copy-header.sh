#!/bin/sh
<<<<<<< HEAD
 
=======

>>>>>>> 4a75ea2543408bd1b2c515aa95901523eeef7858
src_dir="dist/include/"
dst_dir=$1

# Walk through a folder
function foo ()
{
    dir=$1
    for entry in $dir
    do
        if [ -d "$entry" ] ; then
            foo "$entry/*"
        else
            if [ -f "$entry" ]; then
                file="`readlink $entry`"
                if [ ! "$entry" ]; then
                    file="$entry"
                fi
                cp "$file" "$dst_dir/$entry"
            fi
        fi
    done
}

cd $src_dir

<<<<<<< HEAD
foo "*"
=======
foo "*"
>>>>>>> 4a75ea2543408bd1b2c515aa95901523eeef7858
