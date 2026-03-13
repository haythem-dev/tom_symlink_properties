#!/usr/bin/ksh

set -e
#set -x
#zip all in exports

START_DIR=$(pwd)

#paths
PROJECT=tender
EXPORT_PATH=../../export/$PROJECT
PWD_DIR=$(cd $(dirname $0) && pwd)

SRC_DIR=${PWD_DIR}

rm -rf $EXPORT_PATH

mkdir -p ${EXPORT_PATH}/bin
mkdir -p ${EXPORT_PATH}/ini
mkdir -p ${EXPORT_PATH}/proc
mkdir -p ${EXPORT_PATH}/vars

#copy files
cp ${SRC_DIR}/tenderordercreator/build/release/tenderordercreator.bin ${EXPORT_PATH}/bin/
cp ${SRC_DIR}/tenderordercreator/unix/ini/tenderordercreator.cfg ${EXPORT_PATH}/ini/
cp ${SRC_DIR}/tenderordercreator/unix/proc/tenderordercreator.sh ${EXPORT_PATH}/proc/
cp ${SRC_DIR}/tenderordercreator/unix/proc/tenderordercreator.option ${EXPORT_PATH}/proc/
cp ${SRC_DIR}/tenderordercreator/unix/vars/tender_global.vars.* ${EXPORT_PATH}/vars/
cp ${SRC_DIR}/tenderbatchprocessing/build/release/tenderbatchprocessing.bin ${EXPORT_PATH}/bin/
cp ${SRC_DIR}/tenderbatchprocessing/unix/ini/tenderbatchprocessing.cfg ${EXPORT_PATH}/ini/
cp ${SRC_DIR}/tenderbatchprocessing/unix/proc/tenderbatchprocessing.sh ${EXPORT_PATH}/proc/
cp ${SRC_DIR}/tenderbatchprocessing/unix/proc/tenderbatchprocessing.option ${EXPORT_PATH}/proc/


cd $EXPORT_PATH
TARDIRS=$(ls -d *)
tar -czf ./$PROJECT.tar.gz $TARDIRS
rm -rf $TARDIRS

cp ${SRC_DIR}/ivy.xml ${EXPORT_PATH}
cp ${SRC_DIR}/build.xml ${EXPORT_PATH}






cd $START_DIR

#paths
PROJECT=libtender
EXPORT_PATH=../../export/$PROJECT
PWD_DIR=$(cd $(dirname $0) && pwd)

SRC_DIR=${PWD_DIR}

rm -rf $EXPORT_PATH

mkdir -p ${EXPORT_PATH}/include
mkdir -p ${EXPORT_PATH}/lib

#copy files
rsync -auv ./libtender/* ${EXPORT_PATH}/include --include='*/' --include='*.h' --include='*.hpp' --include='*.inl' --exclude='*' --prune-empty-dirs
cp -r $SRC_DIR/libtender/build/debug ${EXPORT_PATH}/lib
cp -r $SRC_DIR/libtender/build/release ${EXPORT_PATH}/lib

cd $EXPORT_PATH
zip -r ./$PROJECT.zip lib include
rm -rf $TARDIRS

#move libtender to tender artifact
mv $PROJECT.zip ../tender/
cd ..
rm -rf $PROJECT