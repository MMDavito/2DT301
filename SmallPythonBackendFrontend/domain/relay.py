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
    def __init__(self, id: int, relay_is_on: bool, duration):
        self.id = id
        self.relay_is_on = relay_is_on
        self.duration = duration

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
        string = string = "led"+str(self.id)+" : duration : " + \
            str(self.duration)+", isOn : "+str(isOn)
        return string

    def toDict(self):
        relay = {
            "id": self.id,
            "relay_is_on": self.relay_is_on,
            "duration": self.duration
        }
        return relay

    def toDictNoBOOOOL(self):
        # TODO: return simple if header "IS_ARDUINO" exists and is true.
        relay = {
            "id": self.id,
            "duration": self.duration
        }
        return relay
