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

copynewversion tenderordercreator/build/release/tenderordercreator.bin  $WSS/release/bin/tenderordercreator.bin;
copynewversion tenderordercreator/unix/proc/tenderordercreator.sh       $WSS/release/proc/tenderordercreator.sh;
copynewversion tenderordercreator/unix/proc/tenderordercreator.option   $WSS/release/proc/tenderordercreator.option;
copynewversion tenderordercreator/unix/ini/tenderordercreator.cfg       $WSS/release/ini/tenderordercreator.cfg;
copynewversion tenderordercreator/unix/vars/tender_global.vars.pfr      $WSS/release/vars/tender_global.vars.pfr;
copynewversion tenderordercreator/unix/vars/tender_global.vars.bg      	$WSS/release/vars/tender_global.vars.bg;
copynewversion tenderordercreator/unix/vars/tender_global.vars.rs      	$WSS/release/vars/tender_global.vars.rs;

copynewversion tenderbatchprocessing/build/release/tenderbatchprocessing.bin    $WSS/release/bin/tenderbatchprocessing.bin;
copynewversion tenderbatchprocessing/unix/proc/tenderbatchprocessing.sh         $WSS/release/proc/tenderbatchprocessing.sh;
copynewversion tenderbatchprocessing/unix/proc/tenderbatchprocessing.option     $WSS/release/proc/tenderbatchprocessing.option;
copynewversion tenderbatchprocessing/unix/ini/tenderbatchprocessing.cfg         $WSS/release/ini/tenderbatchprocessing.cfg;
copynewversion tenderbatchprocessing/unix/vars/tender_global.vars.ch            $WSS/release/vars/tender_global.vars;

echo "finished.";
