#!/usr/bin/sh
# set -x
###############################################################################################
# installs tender applications to $WSS
###############################################################################################

# targets
TARGET_TENDERORDERCREATOR=tenderordercreator
TARGET_TENDERBATCHPROCESSING=tenderbatchprocessing

# subtargets
PROJECT_TENDERORDERCREATOR=./tenderordercreator
PROJECT_TENDERBATCHPROCESSING=./tenderbatchprocessing


COUNTRIES="at bg ch de de4 fr pfr hr rs"	# pat pch pfr: not all applications...

###############################################################################################

function check_create_dir
{
	DIRECTORY=$1

	if [[ ! -d $DIRECTORY ]]; then
		mkdir -p $DIRECTORY;
	fi
}

###############################################################################################

function create_dirs_country
{
	COUNTRY=$1
	TENDER_DIR=$HOME/wss/$COUNTRY/tender
	SUBDIRS="bin data ini log proc vars"

	for dir in $SUBDIRS; do
		check_create_dir $TENDER_DIR/$dir
	done
}

function install_tenderordercreator_country
{
	COUNTRY=$1
	GLOBAL_TENDER_DIR=$WSS/$COUNTRY/tender

	echo "###################################################################"
	echo "# installing $TARGET_TENDERORDERCREATOR to $GLOBAL_TENDER_DIR..."
	echo "###################################################################"
	echo

	create_dirs_country $COUNTRY

	# bin directory
	cp -f $PROJECT_TENDERORDERCREATOR/build/release/tenderordercreator.bin 	$GLOBAL_TENDER_DIR/bin/tenderordercreator.bin

	# proc directory
	cp -f $PROJECT_TENDERORDERCREATOR/unix/proc/tenderordercreator.sh 		$GLOBAL_TENDER_DIR/proc/tenderordercreator.sh
	cp -f $PROJECT_TENDERORDERCREATOR/unix/proc/tenderordercreator.option 	$GLOBAL_TENDER_DIR/proc/tenderordercreator.option
	cp -f $PROJECT_TENDERORDERCREATOR/unix/proc/basar.config.xml 			$GLOBAL_TENDER_DIR/bin/basar.config.xml

	# ini directory
	cp -f $PROJECT_TENDERORDERCREATOR/unix/ini/tenderordercreator.cfg 		$GLOBAL_TENDER_DIR/ini/tenderordercreator.cfg

	# ini directory
	cp -f $PROJECT_TENDERORDERCREATOR/unix/vars/tender_global.vars.$COUNTRY $GLOBAL_TENDER_DIR/vars/tender_global.vars
}

function install_tenderbatchprocessing_country
{
	COUNTRY=$1
	GLOBAL_TENDER_DIR=$WSS/$COUNTRY/tender

	echo "###################################################################"
	echo "# installing $TARGET_TENDERBATCHPROCESSING to $GLOBAL_TENDER_DIR..."
	echo "###################################################################"
	echo

	create_dirs_country $COUNTRY

	# bin directory
	cp -f $PROJECT_TENDERBATCHPROCESSING/build/release/tenderbatchprocessing.bin $GLOBAL_TENDER_DIR/bin/tenderbatchprocessing.bin

	# proc directory
	cp -f $PROJECT_TENDERBATCHPROCESSING/unix/proc/tenderbatchprocessing.sh 	$GLOBAL_TENDER_DIR/proc/tenderbatchprocessing.sh
	cp -f $PROJECT_TENDERBATCHPROCESSING/unix/proc/tenderbatchprocessing.option $GLOBAL_TENDER_DIR/proc/tenderbatchprocessing.option
	cp -f $PROJECT_TENDERBATCHPROCESSING/unix/proc/basar.config.xml 			$GLOBAL_TENDER_DIR/bin/basar.config.xml

	# ini directory
	cp -f $PROJECT_TENDERBATCHPROCESSING/unix/ini/tenderbatchprocessing.cfg $GLOBAL_TENDER_DIR/ini/tenderbatchprocessing.cfg

	# ini directory
	cp -f $PROJECT_TENDERBATCHPROCESSING/unix/vars/tender_global.vars.$COUNTRY $GLOBAL_TENDER_DIR/vars/tender_global.vars
}

function install_all_tenderordercreator
{
	for country in $COUNTRIES; do
		install_tenderordercreator_country $country
	done
}

function install_all_tenderbatchprocessing
{
	for country in $COUNTRIES; do
		install_tenderbatchprocessing_country $country
	done
}

###############################################################################################
# install to $WSS
###############################################################################################


if [[ $# == 1 ]]; then
	COUNTRY=$1
	install_tenderordercreator_country $COUNTRY
	install_tenderbatchprocessing_country $COUNTRY
else
	echo "missing country!!!"
fi
