class Relay:
    def __init__(self, id: int, relay_is_on: bool):
        self.id = id
        self.relay_is_on = relay_is_on

    @classmethod
    def returnJson(cls, filename):
        return "BAJS"

    def toString(self):
        """
        Does not include newline.
        """
        isOn = 0
        if self.relay_is_on:
            isOn = 1
        string = "led"+str(self.id)+" : "+str(isOn)
        return string

    def toDict(self):
        relay = {
            "id": self.id,
            "relay_is_on": self.relay_is_on
        }
        return relay


class RelayDynamic:
    # NEED TO EXTEND IT LATER TO SUPPORT STARTTIMES
    def __init__(self, id: int, relay_is_on: bool, duration, start_time, shitty_time):
        self.id = id
        self.relay_is_on = relay_is_on
        self.duration = duration
        self.start_time = start_time
        self.shitty_time = shitty_time

    @classmethod
    def returnJson(cls, filename):
        return "BAJS"

    @classmethod
    def initFromLine(cls, line: str):
        isOn = True
        if(line[-2] == "0"):
            isOn = False

        index = line.find('d')+1
        indexEnd = line.find(':')-1
        
        subString = line[indexEnd+3:]
        tempStart = subString.find(':')+2
        tempEnd = subString.find(',')
        duration = int(subString[tempStart:tempEnd])#Int is same as arduino long.
        #temp = RelayDynamic(line[index:indexEnd],isOn,duration)

        subString = subString[tempEnd+2:]
        tempStart = subString.find(':')+2
        tempEnd = subString.find(',')
        print("BAJS REMOVE 1: ", subString[tempStart:tempEnd])
        startTime = int(subString[tempStart:tempEnd])

        subString = subString[tempEnd+2:]
        tempStart = subString.find(':')+2
        tempEnd = subString.find(',')
        print("BAJS REMOVE 2: ", subString[tempStart:tempEnd])

        shittyTime = subString[tempStart:tempEnd]
        #temp = RelayDynamic()
        instance = cls(line[index:indexEnd], isOn,
                       duration, startTime, shittyTime)
        return instance

    def toString(self):
        """
        Does not include newline.
        """
        isOn = 0
        if self.relay_is_on:
            isOn = 1
        string = string = "led"+str(self.id)+" : duration : " + \
            str(self.duration)+", start_time : "+str(self.start_time) + \
            ", shitty_time : "+str(self.shitty_time)+", isOn : "+str(isOn)
        return string

    def toDict(self):
        relay = {
            "id": self.id,
            "relay_is_on": self.relay_is_on,
            "duration": self.duration,
            "start_time": self.start_time,
            "shitty_time": self.shitty_time
        }
        return relay

    def toDictNoBOOOOL(self):
        # TODO: return simple if header "IS_ARDUINO" exists and is true.
        relay = {
            "id": self.id,
            "duration": self.duration,
            "start_time": self.start_time
        }
        return relay
