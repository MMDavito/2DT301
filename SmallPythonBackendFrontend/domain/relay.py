class  Relay:
    def __init__(self,id:int,relay_is_on:bool):
        self.id = id
        self.relay_is_on=relay_is_on
    @classmethod
    def returnJson(cls,filename):
        return "BAJS"

    def toDict(self):
        relay = {
            "id": self.id,
            "relay_is_on": self.relay_is_on
        }
        return relay