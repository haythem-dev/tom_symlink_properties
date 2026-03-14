#! /usr/bin/ksh

function copynewversion
{
    local readonly _SOURCE=${1};
    local readonly _TARGET=${2};
    echo "copying ${_SOURCE} to ${_TARGET}";
    rm ${_TARGET};
    cp ${_SOURCE} ${_TARGET};
    chmod 777 ${_TARGET};
    touch ${_TARGET};
}

	
copynewversion build/release/cscbatchhistory.bin   $WSS/release/bin/cscbatchhistory.bin;
copynewversion proc/cscbatchhistory.sh             $WSS/release/proc/cscbatchhistory.sh;
copynewversion proc/cscbatchhistory.option         $WSS/release/proc/cscbatchhistory.option;
copynewversion ini/cscbatchhistory.cfg             $WSS/release/ini/cscbatchhistory.cfg;

echo "finished.";

