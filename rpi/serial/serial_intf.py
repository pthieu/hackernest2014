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

port = serial.Serial("/dev/ttyACM0", baudrate=9600, timeout=3.0)
#port = serial.Serial("/dev/tty.usbmodemfa131", baudrate=9600, timeout=3.0)

numArguments = len(sys.argv)

if numArguments <= 1:
	print "Invalid arguments\n"
	sys.exit(1)
else:
	action = str(sys.argv[1])
	#print "Performing action '%s'" % action
	#print "Have '%d' arguments" % numArguments

numBytesWritten = 0
# receive data from arduino
if numArguments == 2:
	if (action!="record"):
		print "Invalid command, expecting 'record'\n"
		sys.exit(1)
	else:
		numBytesWritten += port.write("<1>");
		#print "Number of bytes written: '%d'" % numBytesWritten
		recvStr = readline(port)
		print (recvStr)

# send a string of data to arduino	
elif numArguments == 3:
	if (action!="send"):
		#print "Invalid command, expecting 'send'\n"
		sys.exit(1)
	else:
		command = str(sys.argv[2])
		sendStr	= "<2><%s>" % command
		#print "Sending command '%s'\n" % sendStr
		numBytesWritten += port.write(sendStr)
		#print "Number of bytes written: '%d'" % numBytesWritten

else:
	sys.exit(1)