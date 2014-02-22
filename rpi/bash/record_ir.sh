# bash script to Record IR Cmd: 
# Input: ID
# Output: write to table in “IR Cmd File”
# Process: Execute “Communication Processor” and pool for return

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

IR_NAME=$1
IR_FILE="../config/ir_cmd.config"
IR_CMD="0xFF"
FIND_NAME=0
echo $IR_NAME
# call communication processor
# record result to file
if [ -e "$IR_FILE" ]; 
then
    # check if there exist IR_NAME already
    FIND_NAME=`grep -w $IR_NAME $IR_FILE -c`
    if [ "$FIND_NAME" -gt "0" ]; 
    then
        echo $IR_NAME exist
        # replace $IR_NAME line
        sed -i "s/^$IR_NAME.*/$IR_NAME $IR_CMD/" "$IR_FILE"
    else
        echo "$IR_NAME $IR_CMD" >> $IR_FILE
    fi
else
    # create the file and write the first command
    echo creating file
    echo "$IR_NAME $IR_CMD" >> $IR_FILE
fi
