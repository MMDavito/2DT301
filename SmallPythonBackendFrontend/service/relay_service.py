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
        code = 200#OK
        return successToResponse.getResponseWithData(data,code)
        
    @classmethod
    def get_relays_datafile(cls):
        """
        Returns list of data (line by line) from the datafile.
        """
        with open(fileName, "r") as myfile:
            lines = myfile.readlines()
            if IS_DEBUG:
                print("relayFileContents: ",lines)
        return lines
