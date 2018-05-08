#!/bin/bash
ROOT=$(readlink -f `dirname $BASE_SOURCE[0]`)
DIRBUILD='build'

if -d "$ROOT/$DIRBUILD" then
    rm -rf "$ROOT/$DIRBUILD"
fi

