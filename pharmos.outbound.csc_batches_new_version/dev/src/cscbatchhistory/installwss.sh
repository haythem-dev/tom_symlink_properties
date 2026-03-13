#!/usr/bin/sh
# set -x
###############################################################################################
# installs cscbatchhistory applications to $WSS
###############################################################################################

# targets
TARGET_CSCBATCHHISTORY=cscbatchhistory

# subtargets
PROJECT_CSCBATCHHISTORY=./


function install_cscbatchhistory_country
{
	COUNTRY=$1
	GLOBAL_CSCBATCHESHISTORYDIR_DIR=$WSS/$COUNTRY/ksc/load/

	echo "###################################################################"
	echo "# installing $TARGET_TRANSFER2BFARM to $GLOBAL_CSCBATCHESHISTORYDIR_DIR..."
	echo "###################################################################"
	echo

	# bin directory
	cp -f $PROJECT_CSCBATCHHISTORY/build/release/cscbatchhistory.bin	$GLOBAL_CSCBATCHESHISTORYDIR_DIR/bin/cscbatchhistory.bin
	
	# proc directory
	cp -f $PROJECT_CSCBATCHHISTORY/proc/cscbatchhistory.sh				$GLOBAL_CSCBATCHESHISTORYDIR_DIR/proc/cscbatchhistory.sh
	cp -f $PROJECT_CSCBATCHHISTORY/proc/cscbatchhistory.option			$GLOBAL_CSCBATCHESHISTORYDIR_DIR/proc/cscbatchhistory.option
	
	# ini directory
	#cp -f $PROJECT_CSCBATCHHISTORY/ini/cscbatchhistory.cfg				$GLOBAL_CSCBATCHESHISTORYDIR_DIR/ini/cscbatchhistory.cfg
}

###############################################################################################
# install to $WSS
###############################################################################################

if [[ $# == 1 ]]; then
	COUNTRY=$1	
	install_cscbatchhistory_country $COUNTRY
else
	echo "missing country!!!"
fi
