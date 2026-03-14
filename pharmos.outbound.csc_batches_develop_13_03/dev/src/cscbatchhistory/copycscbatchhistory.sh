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

function install_country
{
    local readonly _COUNTRY=${1};
	
	copynewversion build/release/cscbatchhistory.bin	$WSS/$_COUNTRY/ksc/load/bin/cscbatchhistory.bin ;
	copynewversion proc/cscbatchhistory.sh				$WSS/$_COUNTRY/ksc/load/proc/cscbatchhistory.sh ;
	copynewversion proc/cscbatchhistory.option			$WSS/$_COUNTRY/ksc/load/proc/cscbatchhistory.option ;
	copynewversion ini/cscbatchhistory.cfg				$WSS/$_COUNTRY/ksc/load/ini/cscbatchhistory.cfg ;
}

install_country "de"

echo "finished.";

