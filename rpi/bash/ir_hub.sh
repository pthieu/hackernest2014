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

runpath="`pwd`/rpi/bash"
echo $runpath;

CMD=$1
NAME=$2
case $CMD in
    send )
        echo send
        # kick off bash
        $runpath/ir_send.sh "$NAME"
        ;;
    record )
        echo record
        # kick off bash
        chk_arg $NAME
        # echo $NAME
        $runpath/ir_record.sh "$NAME"
        ;;
    macro )
        ;;
esac

