#! /usr/bin/ksh

#set -x

CWD=`pwd`
EXPORT_DIR=${CWD}/../../export/csc_batches
BINSOURCE="release"

function copynewversion
{
	local readonly _SOURCE=${1};
	local readonly _TARGET=${2};
	echo "   copying ${_SOURCE} to ${_TARGET}";
	rm -rf ${_TARGET};
	cp -f ${_SOURCE} ${_TARGET};
	chmod 777 ${_TARGET};	
	touch ${_TARGET};
}


rm -rf ${EXPORT_DIR}
mkdir -p ${EXPORT_DIR}


function copy_binaries
{
	BATCH_BIN_DIR=${EXPORT_DIR}/batch/bin/release
	LOAD_BIN_DIR=${EXPORT_DIR}/load/bin/release
	mkdir -p ${BATCH_BIN_DIR}
	mkdir -p ${LOAD_BIN_DIR}
	echo "Copy binaries... "

	copynewversion "${CWD}/CSC_Batches_Bi_Export/csc_batches_bi_export.bin"									"${BATCH_BIN_DIR}/csc_batches_bi_export.bin";
	copynewversion "${CWD}/CSC_Batches_Edi_Input/build/${BINSOURCE}/csc_batches_edi_input.bin"				"${BATCH_BIN_DIR}/csc_batches_edi_input.bin";
	copynewversion "${CWD}/CSC_Batches_OrderProposal/build/${BINSOURCE}/csc_batches_orderproposal.bin"		"${BATCH_BIN_DIR}/csc_batches_orderproposal.bin";
	copynewversion "${CWD}/cscbatchauto/build/${BINSOURCE}/cscbatchauto.bin" 								"${BATCH_BIN_DIR}/cscbatchauto.bin";

	copynewversion "${CWD}/cscbatchhistory/build/${BINSOURCE}/cscbatchhistory.bin" 							"${LOAD_BIN_DIR}/cscbatchhistory.bin";
}


###############################################################################################################################
# copy files to SCRIPTS_TARGET
###############################################################################################################################
function copy_scripts
{
	BATCH_PROC_DIR=${EXPORT_DIR}/batch/proc
	LOAD_PROC_DIR=${EXPORT_DIR}/load/proc
	mkdir -p ${BATCH_PROC_DIR}
	mkdir -p ${LOAD_PROC_DIR}
	echo "Copy scripts... "

	copynewversion "${CWD}/CSC_Batches_Bi_Export/csc_batches_bi_export.sh"									"${BATCH_PROC_DIR}/csc_batches_bi_export.sh";
	copynewversion "${CWD}/CSC_Batches_Bi_Export/csc_batches_bi_export.option"								"${BATCH_PROC_DIR}/csc_batches_bi_export.option";
	copynewversion "${CWD}/CSC_Batches_Edi_Input/unix/csc_batches_edi_input.sh" 							"${BATCH_PROC_DIR}/csc_batches_edi_input.sh";
	copynewversion "${CWD}/CSC_Batches_Edi_Input/unix/csc_batches_edi_input.option" 						"${BATCH_PROC_DIR}/csc_batches_edi_input.option";
	copynewversion "${CWD}/CSC_Batches_OrderProposal/unix/csc_batches_orderproposal.sh"						"${BATCH_PROC_DIR}/csc_batches_orderproposal.sh";
	copynewversion "${CWD}/CSC_Batches_OrderProposal/unix/csc_batches_orderproposal.option"					"${BATCH_PROC_DIR}/csc_batches_orderproposal.option";
	copynewversion "${CWD}/cscbatchauto/proc/cscbatchauto.sh"												"${BATCH_PROC_DIR}/cscbatchauto.sh";
	copynewversion "${CWD}/cscbatchauto/proc/cscbatchauto.option"											"${BATCH_PROC_DIR}/cscbatchauto.option";

	copynewversion "${CWD}/cscbatchhistory/proc/cscbatchhistory.sh"											"${LOAD_PROC_DIR}/cscbatchhistory.sh";
	copynewversion "${CWD}/cscbatchhistory/proc/cscbatchhistory.option"										"${LOAD_PROC_DIR}/cscbatchhistory.option";
}

###############################################################################################################################
# copy files to INI_TARGET
###############################################################################################################################
function copy_inis
{
	BATCH_INI_DIR=${EXPORT_DIR}/batch/ini
	LOAD_INI_DIR=${EXPORT_DIR}/load/ini
	mkdir -p ${BATCH_INI_DIR}
	mkdir -p ${LOAD_INI_DIR}
	echo "Copy inis... "

	copynewversion "${CWD}/CSC_Batches_Edi_Input/unix/csc_batches_edi_input.cfg" 							"${BATCH_INI_DIR}/csc_batches_edi_input.cfg";
	copynewversion "${CWD}/CSC_Batches_OrderProposal/unix/csc_batches_orderproposal.cfg"					"${BATCH_INI_DIR}/csc_batches_orderproposal.cfg";
	copynewversion "${CWD}/cscbatchauto/ini/cscbatchauto.cfg"												"${BATCH_INI_DIR}/cscbatchauto.cfg";

	copynewversion "${CWD}/cscbatchhistory/ini/cscbatchhistory.cfg"											"${LOAD_INI_DIR}/cscbatchhistory.cfg";
}

###############################################################################################################################
# copy files to VARS_TARGET
###############################################################################################################################
function copy_vars
{
	BATCH_VARS_DIR=${EXPORT_DIR}/batch/vars
	LOAD_VARS_DIR=${EXPORT_DIR}/load/vars
	mkdir -p ${BATCH_VARS_DIR}
	mkdir -p ${LOAD_VARS_DIR}
	echo "Copy inis... "
}

###############################################################################################################################
# create csc_core_applications artifact
###############################################################################################################################
copy_binaries
copy_scripts
copy_inis
copy_vars

TARBALL=csc_batches.tar.gz

echo "--- tar ${EXPORT_DIR}/${TARBALL} ..."
cd ${EXPORT_DIR}
TARDIRS=`ls -d */`
gtar -czf ./${TARBALL} ${TARDIRS}
rm -rf ${TARDIRS}
echo "--- tarball contains:"
gtar -tzf ./${TARBALL} ${TARDIRS}
cd ${CWD}

cp -f ivy.xml		${EXPORT_DIR}
cp -f build.xml 	${EXPORT_DIR}
