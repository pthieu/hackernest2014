# Run IR Cmd/Macro:
# Input: ARG[0]: name of cmd ID or macro id
# Output: N/A
# Process: 
# read file, find name, extract IR code
# Execute “Communication processor” for each IR code (non blocking)
# macro name;; sub macro names;; ir cmd(s);; number of time used
# ( sub macro names omitted if macro is not composed of macros. ie the first ir
# cmd)

#!/bin/bash -x

# 1 arg
function chk_arg
{
    if [ -z "$1" ]; then
        echo missing argument
        exit
    fi
}

runpath="`pwd`/rpi"

chk_arg $1
FILE="$runpath/config/macro.config"
NAME=$1
COM_PROC="$runpath/serial/serial_intf.py send"

# check if file exist
if [ -e $FILE ];
then
    echo file found
    # look for name in the file
    echo $NAME
    MACRO_ENTRY=`grep -w "^$NAME::" $FILE`
    echo $MACRO_ENTRY
    CMD=`echo $MACRO_ENTRY | eval "sed -n -e 's/^"$NAME"::.*::\(.*\)::.*$/\1/p'"`
    chk_arg $CMD
    COUNTER=`echo $MACRO_ENTRY | eval "sed -n -e 's/^"$NAME"::.*::.*::\(.*\)$/\1/p'"`
    chk_arg $COUNTER
    echo $CMD
    COUNTER=$(($COUNTER+1))
    eval "sed -i -e 's/\(^"$NAME"::.*::.*::\).*$/\1 "$COUNTER"/' $FILE"
    echo $COUNTER
    
    for i in $CMD;
    do
        #echo Send $i
        python $COM_PROC $CMD
    done
else
    echo file found
    exit
fi

