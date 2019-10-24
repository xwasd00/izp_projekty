#!/bin/bash

APP=./proj2
DIFF=diff
TESTS_DIR=./tests
IN_NAME=in
OUT_NAME=out
TMPFILE=tmp_out

function test() {
  TEST_PATH=$1
  NAME=$(basename ${TEST_PATH})
  IN=$(< ${TEST_PATH}/${IN_NAME})
  OUT=${TEST_PATH}/${OUT_NAME}
  OUT_CONTENT=$(cat ${OUT})
  ${APP} ${IN} > ${TMPFILE} 2> /dev/null
  rc=$?
  if [ "${OUT_CONTENT}" = "error" ]; then
	if [[ $rc -ne 0 ]]; then
		echo "${NAME} - OK"
	else
		echo "${NAME} - FAILED"
		echo "	Error occured and zero was returned. A nonzero digit should be returned."
	fi
  else
	DIFF_RESULT=$(${DIFF} ${OUT} ${TMPFILE})
	if [ -z "${DIFF_RESULT}" ]; then
		echo "${NAME} - OK"
	else
		echo "${NAME} - FAILED"
		echo ${DIFF_RESULT}
	fi
  fi
  rm ${TMPFILE}
}

for file in $(ls ${TESTS_DIR});
do
  test $TESTS_DIR/${file}
done
