var/lib/dpkg/lock-frontend	
IPV4:
<!-->10.42.0.44</!-->
RASPI LOCALHOST:
192.168.0.181
IPV6:
2001:9b1:23ff:eb01:ba27:ebff:fe1b:54b1
OR:
fe80::ba27:ebff:fe1b:54b1

Router ipv4 GLOBAL:
85.24.161.85

RASPI SERVER GLOBAL SSH:
85.24.161.85 PORT:22

RASPI SERVER pythonFlask:
85.24.161.85:5000

portExternal:
987
internal: 7

Port TCP: 985
Port UDP: 986




ping 192.168.0.181 -c 5

ping 85.24.161.85 -c 5


ssh same!


ping 85.24.161.85 -c 5



ping6 2001:9b1:23ff:eb01:ba27:ebff:fe1b:54b1 -c 5

ssh ubuntu@192.168.0.181
ssh ubuntu@85.24.161.85

**REMOTE:**
scp -r ~/LNU/2DT301/SmallPythonBackendFrontend/ ubuntu@85.24.161.85:/home/ubuntu/LNU/2DT301/
**LOCAL:**
scp -r ~/LNU/2DT301/SmallPythonBackendFrontend/ ubuntu@192.168.0.181:/home/ubuntu/LNU/2DT301/



