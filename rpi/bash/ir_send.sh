# Run IR Cmd/Macro:
# Input: ARG[0]: IR code(s) file name,
#           ARG[1]: name of cmd ID or macro id
# Output: N/A
# Process: 
# read file, find name, extract IR code
# Execute “Communication processor” for each IR code (non blocking)

#!/bin/bash -x

# 1 arg
function chk_arg
{
    if [ -z "$1" ]; then
        echo missing argument
        exit
    fi
}

chk_arg $1
chk_arg $2
FILE="../config/$1"
NAME=$2
COM_PROC="../serial/serial_intf.py send"

# check if file exist
if [ -e $FILE ];
then
    echo file found
    # look for name in the file
    echo $NAME
    CMD=`grep -w "$NAME:" $FILE | sed s/"$NAME:"//`
    chk_arg $CMD
    echo $CMD
    
    for i in $CMD;
    do
        echo Send $i
        python $COM_PROC $CMD
    done
else
    echo file found
    exit
fi

