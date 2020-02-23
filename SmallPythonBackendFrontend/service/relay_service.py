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
            index = line.find('d')+1
            indexEnd = line.find(':')-1
            if IS_DEBUG:
                print("Line data on off?: ",line[-2])
            isOn = True
            if(line[-2]=="0"):
                isOn=False
            temp = Relay(line[index:indexEnd],isOn)
            listRelays.append(temp.toDict())
            i+=1
        return listRelays
    @classmethod
    def postRelay(cls,api_key,data):
        return cls.postRelayToFile(data)
    @classmethod
    def postRelayToFile(cls,data):
        """
        Prints/replaces information in file....
        """
        #dataParsed =json.loads(data)
        #relays = json.loads(dataParsed["relays"])
        if "is_dynamic" in data and (not data["is_dynamic"] is None) and data["is_dynamic"] == True:
            print("SHISEEEE")
            eCode = 400
            msg = "Bad request"
            return errorsToResponse.getResponse(msg,eCode)
        relays = (data["relays"])
        if IS_DEBUG:
            print("RELAYS HEYBARBARIBA type: ",type(relays),", Value: ",relays)

        for i in range(len(relays)):
            temp = Relay(**relays[i])
            relays[i] = temp
            if IS_DEBUG:
                print("TEMP type: ",type(temp),", temp value: ",temp)

        with open(fileName, "r") as myfile:
            lines = myfile.readlines()
            if IS_DEBUG:
                print("relayFileContents: ",lines)
        
        i=0
        for line in lines:
            index = line.find('d')+1
            indexEnd = line.find(':')-1
            if IS_DEBUG:
                print("Relay index from file?: ", index,", and its value: ",line[index:indexEnd])
            i+=1

        i = 0
        with open(fileName, "w") as myfile:
            for i in range(len(relays)):
                isOn = 0
                if IS_DEBUG:
                    print("RELAY NUMBER: ",relays[i].id,", uses: "+str(relays[i].relay_is_on))
                if relays[i].relay_is_on:
                    isOn = 1
                myfile.write("led"+str(relays[i].id)+" : "+str(isOn))
                myfile.write("\n")
        code = 202
        msg = "majestetic"
        return successToResponse.getResponseWithMessage(msg,code)