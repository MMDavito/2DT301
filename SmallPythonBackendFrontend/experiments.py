from datetime import datetime
print(type(datetime.now().time()))
print(datetime.now().time())
print(type(str(datetime.utcnow().time())))
print(str(datetime.utcnow().time()))

time = datetime.utcnow().time()


print(str(time))

time_form = "%H:%M:%S"
print(time.strftime(time_form))
