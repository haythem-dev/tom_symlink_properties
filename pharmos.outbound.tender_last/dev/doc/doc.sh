#!/bin/ksh
set -x

rm -rf ./codedocu
rm -rf doxy_warn.txt

mkdir ./codedocu
mkdir ./codedocu/internal
mkdir ./codedocu/customer

DOXYGEN=$DEVLIB_PATH/marathon/bin/doxygen/doxygen
$DOXYGEN -d Preprocessor Doxyfile.internal
$DOXYGEN -d Preprocessor Doxyfile.customer

#HOST='denu1vc0029.phoenix.loc'
HOST='10.158.54.114'
USER='ftp.outbound'
PASSWD='stepRU7e'

ftp -n -i <<END_SCRIPT
open $HOST
user $USER $PASSWD 
cd tender/internal
lcd codedocu/internal/html
put index.html
put pages.html
put versionoverview.html
put versionhistory.html
cd ../customer
lcd ../../customer/html
put index.html
disconnect
quit
END_SCRIPT

exit 0
