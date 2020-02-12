Skicka mail om ej kommmer på seminarium (måndagar)
---------------------

1. Install server and allow ssh file transfers.
2. Find out if i can install gpio and turn on a single led(with raspberry ubuntu server using python gpio)
3. If possible, create internet based IoT device on localhost.
4. create server that can take n number of registered users that can have m number of devices.
5. make this server able to controll relay/light/solenoid valves.
6. also take and store temp and moisture.


Alternativ lösning är att ha en raspberry för server och en för "Embeded systems"
För den centrala servern skall ha server ability att vara flexibel och embeded py bör vara en prototyp på en av dessa "m" antal IoT devices.



-----------------------------
Vecka 7:
Skapa hello world install over ssh.
temmperatur.

Nu skall jag:
1. få raspberry py att fungera över SSH
2. sätt den vid routern (använd grenuttags-sakmojäng.)
**DONE TO HERE!**

3. Create and upload hello world (over SSH) to the raspberry (python) (list ipaddress and/or name, and a message (can be inputed via webform or posted)
This is then stored in a list on the server (will be reset on restart)), this list can also be displayed on web (or via logs).
BUT FUCK LOGS! stick to temporary storage.

4. Kolla om du kan pinga hello world från arduino using wifi module.
5. Använd docker och seperera databas, server och frontend??????

6. Create database
	1. User database (KornettisVagen, password: "Skall du ge fan i")
	After registration one can register new device: This creates unique random API key(so no klartext password)
	2. DEVICE_database Allow for the devices with this API key to post information (and read intervalls for the relay)
--------------------------------
TODO BEFORE LUNCH:
Basic setup
check via ssh ifconfig
1. ping the raspberry
2. Check if i can ping it from an external network (phone)
If not, open the port untill you can do this and SSH over this external network.







**Backup over SSH to image file, put it inside hdd som sitter i TV:n**

