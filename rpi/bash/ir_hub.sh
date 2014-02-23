# central bash to controll different bash scripts

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

runpath="`pwd`/rpi/bash/"
echo $runpath;

CMD=$1
FILE=$2
NAME=$2
SEND_NAME=$3

case $CMD in
    send )
        echo send
        chk_arg $NAME
        chk_arg $SEND_NAME
        # kick off bash
        ./ir_send.sh $FILE "$SEND_NAME"
        ;;
    record )
        echo record
        # kick off bash
        chk_arg $NAME
        # echo $NAME
        ./record_ir.sh $NAME
        ;;
esac

