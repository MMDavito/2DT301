from flask import Response, json
#from support.connection_factory import Connection
from support.Response_Maker import IS_DEBUG, errorsToResponse, successToResponse,timeHelper
from domain.relay import Relay, RelayDynamic
import mysql.connector
from mysql.connector import errorcode

fileName = "data/relays.txt"


class RelayService():
    @classmethod
    def getRelays(cls, api_key,isArduino):
        data = cls.get_relays_datafile(isArduino)
        #relays = {"relays": data}
        code = 200  # OK
        return successToResponse.getResponseWithData(data, code)

    @classmethod
    def get_relays_datafile(cls,isArduino:bool):
        """
        Returns list of data (line by line) from the datafile.
        """
        if isArduino != True:#Could be any shit
            isArduino = False
        with open(fileName, "r") as myfile:
            lines = myfile.readlines()
            if IS_DEBUG:
                print("relayFileContents: ", lines)

        listRelays = []
        for i in range(0,3):
            if IS_DEBUG:
                print("line: ",i,", gives: ",lines[i])
        dyn=int(lines[0][-2])
        sT=int(lines[1][-2])
        eST=int(lines[2][-2])
        repeats = int(lines[3][(lines[3].find(":")+2):])
        is_dynamic = False
        hasStartTime = False
        eachHasStartTime = False
        if dyn == 1:
            is_dynamic = True
            if sT == 1:
                hasStartTime = True
            if eST == 1:
                eachHasStartTime = True
        
        if IS_DEBUG:
            print("DYN=",dyn,", ST=",sT,", EST=",eST)
        for i in range(4,len(lines)):
            line = lines[i]
            index = line.find('d')+1
            indexEnd = line.find(':')-1
            if IS_DEBUG:
                print("Line data on off?: ", line[-2])
            if is_dynamic:                
                temp = RelayDynamic.initFromLine(str(line))
                if isArduino:
                    listRelays.append(temp.toDictNoBOOOOL())
                else:
                    listRelays.append(temp.toDict())
            else:
                isOn = True
                if(line[-2] == "0"):
                    isOn = False
                temp = Relay(line[index:indexEnd], isOn)#IF IS DYN!!!!!!
                listRelays.append(temp.toDict())
        
        resp = {
            "is_dynamic":is_dynamic,
            "has_start_time":hasStartTime,
            "each_has_start_time":eachHasStartTime,
            "current_time":timeHelper.get_current_timeArduino(),
            "num_repeats":repeats,
            "relays":listRelays
        } 
        """
        if isArduino:
            resp["current_time"]=timeHelper.get_current_timeArduino()
        """
        return resp

    @classmethod
    def postRelay(cls, api_key, data):
        return cls.postRelayToFile(data)

    @classmethod
    def postRelayToFile(cls, data):
        """
        Prints/replaces information in file....
        """
        #dataParsed =json.loads(data)
        #relays = json.loads(dataParsed["relays"])

        is_dynamic = False
        hasStartTime = False
        eachHasStartTime = False
        repeats=-2
        if "is_dynamic" in data and (not data["is_dynamic"] is None) and data["is_dynamic"] == True:
            """
            print("SHISEEEE")
            eCode = 400
            msg = "Bad request"
            return errorsToResponse.getResponse(msg, eCode)
            """
            if IS_DEBUG:
                print("Will this work??, Lets find out!!")
            is_dynamic=True
            if "has_start_time" in data and (not data["has_start_time"] is None) and data["has_start_time"] == True:
                hasStartTime = True
            if "each_has_start_time" in data and (not data["each_has_start_time"] is None) and data["each_has_start_time"] == True:
                eachHasStartTime = True
            if is_dynamic:
                 if "num_repeats" in data and (not data["num_repeats"] is None):
                     repeats=data["num_repeats"]


        relays = (data["relays"])
        if IS_DEBUG:
            print("RELAYS HEYBARBARIBA type: ",
                  type(relays), ", Value: ", relays)

        for i in range(len(relays)):
            if is_dynamic:
                temp = RelayDynamic(**relays[i])
            else:
                temp = Relay(**relays[i])
            relays[i] = temp
            if IS_DEBUG:
                print("TEMP type: ", type(temp), ", temp value: ", temp)

        """
        with open(fileName, "r") as myfile:
            lines = myfile.readlines()
            if IS_DEBUG:
                print("relayFileContents: ", lines)
        i = 0
        for line in lines:
            index = line.find('d')+1
            indexEnd = line.find(':')-1
            if IS_DEBUG:
                print("Relay index from file?: ", index,
                      ", and its value: ", line[index:indexEnd])
            i += 1
        """
        i = 0

        dyn = 0  # DYNAMIC
        sT = 0  # StartTimes
        eST = 0  # eachStartTimes
        if is_dynamic:
            dyn = 1
            if hasStartTime:
                sT = 1
            if eachHasStartTime:
                eST = 1

        with open(fileName, "w") as myfile:
            myfile.write("dyn : "+str(dyn))
            myfile.write("\n")
            myfile.write("sT : "+str(sT))
            myfile.write("\n")
            myfile.write("eST : "+str(eST))
            myfile.write("\n")
            #if is_dynamic:
            myfile.write("repeats : "+str(repeats))
            myfile.write("\n")


            for i in range(len(relays)):
                if IS_DEBUG:
                    print("RELAY NUMBER: ",
                          relays[i].id, ", uses: "+str(relays[i].relay_is_on))

                myfile.write(relays[i].toString())
                myfile.write("\n")
        code = 202
        msg = "majestetic"
        return successToResponse.getResponseWithMessage(msg, code)
