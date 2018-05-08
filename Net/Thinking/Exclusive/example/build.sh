#!/bin/bash
ROOT=$(readlink -f `dirname $BASE_SOURCE[0]`)
DIRBUILD="$ROOT/build"


if -d "$DIRBUILD"; then
    rm -rf "$DIRBUILD"
fi

mkdir -p "$DIRBUILD"

############################
cd $DIRBUILD && cmake .. && make -j2