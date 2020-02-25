Fortsätt på:
http://127.0.0.1:5000/temp/

--------------------
Jag måste ta en sak i taget.
På fredag skall jag rensa och bara använda basic, 2 statiska sidor.
Där jag kan läsa temp och humitidy at reload.

Det blir komplicerat senare när jag skall ha databas på en annan port, men det är ju iallafall på samma system.
Also credentials.

Men först måste jag ju skapa databas.
-------
TLDR:
Jag måste ta en sak i taget.
På fredag skall jag enbart få typ en "Post whatafuck you want".
Kanske läser från "Guestbook.txt" istället för en lista på servern.
Men måste lägga statiskt (ip.this) till min javascript från python
Vilket inte borde vara så svårt (ipn står ju i commandline när man startar)
**GLHF!**

       window.location.href = "/results/"+resp["responseJSON"]["uuid"];  

Tomorrow friday the 2020-02-14:
1. Read from request and print to file.
2. Read from file and return as a list (print)
3. Upload data ((timestamp),temp and moist) with datetime with CREDENTIALS = "bajsig_arduino"
4. put this in another file.
5. Return this data like the things from data/data.txt, but under results/arduino

**NOW BEFORE EATS:**
Få upp skiten på din server och öppna port på lämpligt viss och sedan börja arbeta med arduino

Will after lunch get arduino working over wifi!!!
------------------------------------------------------------
**2020-02-19**
Will need to make the json work on arduino.
But firstly need to make a temporary webpage that can change the data in the datafile.
Then make the arduino to update 1 time per second(or more often). (parrallell? thread sleep?)

When i later implement the database 1 time per minute would be good enough (button for imediate update on the arduino using TRIGGER INTERRUPT??)
But firstly turn on and of led from webpage.
------------------------------
Add "get current time" for the arduino to get current time of day in millis for start of duration....
-------------------------------
**2020-02-25**
Make starttime possible.
Will make server send time of day in milliseconds to arduino when asking for dynamic.....