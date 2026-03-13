#!/usr/bin/sh
# set -x
###############################################################################################
# installs cscbatchesediinput applications to $WSS
###############################################################################################

# targets
TARGET_CSCBATCHHISTORY=cscbatchesediinput

# subtargets
PROJECT_CSCBATCHHISTORY=./


function install_cscbatchesediinput_country
{
	COUNTRY=$1
	GLOBAL_CSCBATCHESEDIINPUT_DIR=$WSS/$COUNTRY/ksc/batch/

	echo "###################################################################"
	echo "# installing $TARGET_CSCBATCHHISTORY to $GLOBAL_CSCBATCHESEDIINPUT_DIR..."
	echo "###################################################################"
	echo

	# bin directory
	cp -f $PROJECT_CSCBATCHHISTORY/build/release/csc_batches_edi_input.bin	$GLOBAL_CSCBATCHESEDIINPUT_DIR/bin/csc_batches_edi_input.bin
	
	# proc directory
	cp -f $PROJECT_CSCBATCHHISTORY/unix/csc_batches_edi_input.sh			$GLOBAL_CSCBATCHESEDIINPUT_DIR/proc/csc_batches_edi_input.sh
	cp -f $PROJECT_CSCBATCHHISTORY/unix/csc_batches_edi_input.option		$GLOBAL_CSCBATCHESEDIINPUT_DIR/proc/csc_batches_edi_input.option
	
	# ini directory
	cp -f $PROJECT_CSCBATCHHISTORY/unix/csc_batches_edi_input.cfg			$GLOBAL_CSCBATCHESEDIINPUT_DIR/ini/csc_batches_edi_input.cfg
}

###############################################################################################
# install to $WSS
###############################################################################################

if [[ $# == 1 ]]; then
	COUNTRY=$1	
	install_cscbatchesediinput_country $COUNTRY
else
	echo "missing country!!!"
fi