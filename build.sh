#!/usr/bin/env bash
#---------------------------------------------------------------------

# Relative paths to the modules which can be built with this script:
readonly SCS_PATH=simple_compr_service/
readonly TC_PATH=test_client/
readonly LCMN_PATH=lib_common/

readonly OUTPUT_DIR=build/

#---------------------------------------------------------------------

function build_module()
{
    cd $1
    if [ ! -d $OUTPUT_DIR ]; then
        mkdir $OUTPUT_DIR
    fi

    cd $OUTPUT_DIR
    cmake ..
    make
    cd ..
}

function clean()
{
    cd $1
    if [ -d $OUTPUT_DIR ]; then
        cd $OUTPUT_DIR
        rm -rf *
    fi
    cd ../..
}

function build_all()
{
    build_module $LCMN_PATH
    cd ..
    build_module $SCS_PATH
    cd ..
    build_module $TC_PATH
}

#---------------------------------------------------------------------

if [ "$#" -ne 1 ]; then
    echo "'$0 1' (or '$0 s') -- build the 'simple_compr_service' binary"
    echo "'$0 2' (or '$0 t') -- build the 'test_client' binary"
    echo "'$0 3' (or '$0 l') -- build the 'lib_common' shared library"
    echo "'$0 4' (or '$0 b') -- build all the stuff above"
    echo "'$0 5' (or '$0 c') -- clean all the built stuff above"
    exit
fi

if [ "$1" == "1" ] || [ "$1" == "s" ]; then
    build_module $SCS_PATH
elif [ "$1" == "2" ] || [ "$1" == "t" ]; then
    build_module $TC_PATH
elif [ "$1" == "3" ] || [ "$1" == "l" ]; then
    build_module $LCMN_PATH
elif [ "$1" == "4" ] || [ "$1" == "b" ]; then
    build_all
elif [ "$1" == "5" ] || [ "$1" == "c" ]; then
    clean $LCMN_PATH
    clean $SCS_PATH
    clean $TC_PATH
else
    echo -e "ERROR: invalid script parameter provided."
    echo "To get help, run this script without parameters."
fi
