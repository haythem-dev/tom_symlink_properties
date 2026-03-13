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

	
copynewversion build/release/csc_batches_edi_input.bin   $WSS/release/bin/csc_batches_edi_input.bin;
copynewversion unix/csc_batches_edi_input.sh             $WSS/release/proc/csc_batches_edi_input.sh;
copynewversion unix/csc_batches_edi_input.option         $WSS/release/proc/csc_batches_edi_input.option;
copynewversion unix/csc_batches_edi_input.cfg            $WSS/release/ini/csc_batches_edi_input.cfg;

echo "finished.";

