from flask import Response, json
#from support.connection_factory import Connection
from support.Response_Maker import IS_DEBUG, errorsToResponse,successToResponse
from domain.relay import Relay
import mysql.connector
from mysql.connector import errorcode

fileName = "data/relays.txt"

class RelayService():
    @classmethod
    def getRelay(cls,api_key):
        data = cls.get_relays_datafile()
        relays = {"relays":data}
        code = 200#OK
        return successToResponse.getResponseWithData(relays,code)
        
    @classmethod
    def get_relays_datafile(cls):
        """
        Returns list of data (line by line) from the datafile.
        """
        with open(fileName, "r") as myfile:
            lines = myfile.readlines()
            if IS_DEBUG:
                print("relayFileContents: ",lines)
                
        listRelays = []
        i=0
        for line in lines:
            if IS_DEBUG:
                print("Line data on off?: ",line[-2])
            isOn = True
            if(line[-2]=="0"):
                isOn=False
            temp = Relay(i,isOn)
            listRelays.append(temp.toDict())
            i+=1
        return listRelays
