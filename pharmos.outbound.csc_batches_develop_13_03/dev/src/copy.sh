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
	
	copynewversion cscbatchauto/build/release/cscbatchauto.bin	$WSS/$_COUNTRY/ksc/batch/bin/cscbatchauto.bin ;
	copynewversion cscbatchauto/proc/basar.config.xml			$WSS/$_COUNTRY/ksc/batch/bin/basar.config.xml ;
	copynewversion cscbatchauto/proc/cscbatchauto.sh			$WSS/$_COUNTRY/ksc/batch/proc/cscbatchauto.sh ;
	copynewversion cscbatchauto/proc/cscbatchauto.option		$WSS/$_COUNTRY/ksc/batch/proc/cscbatchauto.option ;
	copynewversion cscbatchauto/ini/cscbatchauto.cfg			$WSS/$_COUNTRY/ksc/batch/ini/cscbatchauto.cfg ;
	
	copynewversion cscbatchhistory/build/release/cscbatchhistory.bin	$WSS/$_COUNTRY/ksc/load/bin/cscbatchhistory.bin ;
	copynewversion cscbatchhistory/proc/cscbatchhistory.sh				$WSS/$_COUNTRY/ksc/load/proc/cscbatchhistory.sh ;
	copynewversion cscbatchhistory/proc/cscbatchhistory.option			$WSS/$_COUNTRY/ksc/load/proc/cscbatchhistory.option ;
	copynewversion cscbatchhistory/ini/cscbatchhistory.cfg				$WSS/$_COUNTRY/ksc/load/ini/cscbatchhistory.cfg ;
}

install_country "de"

echo "finished.";

