# bash script to Record IR Cmd: 
# Input: ID
# Output: write to table in “IR Cmd File”
# Process: Execute “Communication Processor” and pool for return
# macro.config format:
# macro name:: sub macro names:: ir cmd(s):: number of time used
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

IR_NAME="$1::"
IR_FILE="$runpath/config/macro.config"
IR_CMD=0
FIND_NAME=0
COM_PROC="$runpath/serial/serial_intf.py record"

echo $IR_NAME
# call communication processor
IR_CMD=`python $COM_PROC`
#IR_CMD="BCD"
chk_arg $IR_CMD
echo $IR_CMD

# record result to file
if [ -e "$IR_FILE" ]; 
then
    # check if there exist IR_NAME already
    FIND_NAME=`grep -w "^$IR_NAME" $IR_FILE -c`
    if [ "$FIND_NAME" -gt "0" ]; 
    then
        echo $IR_NAME exist
        # replace $IR_NAME line
        sed -i "s/^$IR_NAME.*/$IR_NAME :: $IR_CMD:: 0/" "$IR_FILE"
    else
        echo "$IR_NAME :: $IR_CMD:: 0" >> $IR_FILE
    fi
else
    # create the file and write the first command
    echo creating file
    echo "$IR_NAME :: $IR_CMD:: 0" >> $IR_FILE
fi
