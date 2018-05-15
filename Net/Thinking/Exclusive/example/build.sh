#!/bin/bash
ROOT=$(readlink -f `dirname $BASE_SOURCE[0]`)
DIRBUILD="$ROOT/build"


if [ -d "$DIRBUILD" ]; then
    rm -rf "$DIRBUILD"
    rm output
fi

mkdir -p "$DIRBUILD"
ln -s "$DIRBUILD/output/" output

############################
cd $DIRBUILD && cmake .. && make -j2
