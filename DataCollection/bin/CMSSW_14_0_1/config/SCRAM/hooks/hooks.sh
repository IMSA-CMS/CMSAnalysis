#!/bin/bash
SCRIPT_DIR=$(dirname $0)
HOOK_TYPE=$(basename $0 | sed -e 's|-hook$||')
export LC_ALL=C
if [ -e ${SCRIPT_DIR}/${HOOK_TYPE} ] ; then
  for hook in $(find ${SCRIPT_DIR}/${HOOK_TYPE} -type f | sort) ; do
    hook_name=$(echo $hook | sed "s|${SCRIPT_DIR}/||")
    if [ "${SCRAM_IGNORE_HOOKS}" != "" -a -e "${SCRAM_IGNORE_HOOKS}" ] ;  then
      grep -q "^${hook_name}$" "${SCRAM_IGNORE_HOOKS}" && continue
    fi
    [ -x $hook ] && $hook
  done
fi
