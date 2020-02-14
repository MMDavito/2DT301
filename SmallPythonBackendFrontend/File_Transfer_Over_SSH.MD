https://hostadvice.com/how-to/how-to-transfer-files-from-one-remote-server-to-another-remote-server-using-ssh/

scp [switch] [source content location] [destination content location]


**REMOTE:**
scp -r ~/LNU/2DT301/SmallPythonBackendFrontend/ ubuntu@85.24.161.85:/home/ubuntu/LNU/2DT301/
**LOCAL:**
scp -r ~/LNU/2DT301/SmallPythonBackendFrontend/ ubuntu@192.168.0.181:/home/ubuntu/LNU/2DT301/


