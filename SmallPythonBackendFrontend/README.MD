To launch:
FLASK_APP=app.py FLASK_DEBUG=1 flask run
FLASK_APP=app.py FLASK_DEBUG=1 flask run --host=192.168.0.181



**How I launched the app**
Combined this:
https://serverfault.com/questions/409812/running-python-app-in-the-background-on-linux
With this:
https://stackoverflow.com/questions/625409/how-do-i-put-an-already-running-process-under-nohup

FLASK_APP=app.py FLASK_DEBUG=1 nohup flask run --host=192.168.0.181
[ctrl+z]
bg
ps aux | grep python3

Do not need following:
disown -h (pid) (both S and Sl)


**How to check size nohup:**
ls -lh nohup.out 

**How to end without reboot**
kill pid
