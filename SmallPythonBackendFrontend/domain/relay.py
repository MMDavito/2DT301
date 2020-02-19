class  Relay:
    def init(self, api_key, relay0:bool,relay1:bool,relay2:bool,relay3:bool):
        self.api_key = api_key
        self.relay0=relay0
        self.relay1=relay1
        self.relay2=relay2
        self.relay3=relay3
    @classmethod
    def returnJson(cls,filename):
        return "BAJS"
