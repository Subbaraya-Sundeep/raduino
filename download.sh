#! /usr/bin/kermit

set line /dev/ttyUSB0
set speed 115200
set serial 8n1

# General C-Kermit settings.  These probably don't need to change.
set flow-control none
set file type bin
set carrier-watch off
set prefixing all
set modem none

echo "Ready to upload power cycle the board.."

input 60 "Hit any key to stop autoboot:"
output " "
input 5 "rpi2-boot> "
lineout "loadb"
echo "sending file.."
send app.bin
input 5 "rpi2-boot>"
lineout "go 0x8000"
connect
