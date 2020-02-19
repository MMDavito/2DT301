class  Relay:
    def __init__(self,id:int,relayIsOn:bool):
        self.id = id
        self.relayIsOn=relayIsOn
    @classmethod
    def returnJson(cls,filename):
        return "BAJS"

    def toDict(self):
        relay = {
            "id": self.id,
            "relay_is_on": self.relayIsOn
        }
        return relay