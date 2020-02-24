from datetime import datetime
print(type(datetime.now().time()))
print(datetime.now().time())
print(type(str(datetime.utcnow().time())))
print(str(datetime.utcnow().time()))

time = datetime.utcnow().time()


print(str(time))

time_form = "%H:%M:%S"
print(time.strftime(time_form))

##########################################
line = "led3 : duration : 4000, isOn : 0\n"
index = line.find('d')+1
indexEnd = line.find(':')-1

subString = line[indexEnd+3:]
tempStart = subString.find(':')+2
tempEnd = subString.find(',')

duration = (subString[tempStart:tempEnd])#Int is same as arduino long.
print("Duration string:"+duration)

duration = int(subString[tempStart:tempEnd])#Int is same as arduino long.
print("Duration integer:"+str(duration))
                
