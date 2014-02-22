#!/usr/bin/python

import serial
import sys

def readline(port):
    rv = ""
    while True:
        ch = port.read()
        rv += ch
        if ch=='\r' or ch=='':
            return rv

port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=3.0)

numArguments = len(sys.argv)

if numArguments > 0:
	action = str(sys.argv[1])
	

# receive data from arduino
if numArguments == 1:
	if (action!="record"):
		sys.exit(1)
	else:
		numBytesWritten += port.write("<1>\n");
		recvStr = readline(port)
		print (recvStr)
	
elif numArguments == 2:
	if (action!="send"):
		sys.exit(1)
	else:
		command = str(sys.argv[2])
		sendStr	= "<2><%s>\n" % command
		numBytesWritten += port.write(sendStr)

else:
	sys.exit(1)