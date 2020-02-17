Followed this to set up wifi:
https://www.kjell.com/se/kunskap/hur-funkar-det/arduino/arduino-projekt/anslut-arduino-till-wifi

10kOhm: {brown black orange}
22kOhm: {red red orange}

Command:
ls -l /dev/ttyACM0
Gives:
crw-rw-rw- 1 root dialout 166, 0 feb 17 11:34 /dev/ttyACM0

sudo adduser david dialout 166
sudo adduser david dialout
