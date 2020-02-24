from flask_restful import Resource,request
from service.relay_service import RelayService
from support.Response_Maker import IS_DEBUG,errorsToResponse,successToResponse
from support.CREDENTIALS import CREDENTIALS, API_KEY
service = RelayService()

class RelayController(Resource):#Should be plural/list.....
    def get(self):
        if IS_DEBUG:
            print("ARDUINO FUCK YES!")
            print("FORMBAJS: ",request.headers)
        apiKey = request.headers["x-api-key"]
    
        credentials = request.headers["Credentials"]
        if IS_DEBUG:
            print("Api key?: ",apiKey)
            print("Credentials?: ",credentials)

        if apiKey != API_KEY or credentials != CREDENTIALS:
            eCode = 401
            eMsg = "Not the correct credentials!"
            return errorsToResponse.getResponse(eMsg,eCode)
        isArduino = False
        if "is_arduino" in request.headers:
            queArduino = request.headers["is_arduino"]#IS ARDUINO???
            if queArduino=="true":
                isArduino  = True#else remains unchanged
                
        #ELSE:
        return service.getRelays(apiKey,isArduino)
    def post(self):
        if IS_DEBUG:
            print("ARDUINO FUCK YES!")
            print("FORMBAJS: ",request.headers)
        
        apiKey = request.headers["x-api-key"]
        credentials = request.headers["Credentials"]
        if IS_DEBUG:
            print("Api key?: ",apiKey)
            print("Credentials?: ",credentials)

        if apiKey != API_KEY or credentials != CREDENTIALS:
            eCode = 401
            eMsg = "Not the correct credentials!"
            return errorsToResponse.getResponse(eMsg,eCode)
        data = request.json
        if data == None:
            eCode = 400
            eMsg = "Incorrect formatt/Bad request!"
            return errorsToResponse.getResponse(eMsg,eCode)
        #ELSE:
        #if

        if IS_DEBUG:
            print("Api key?: ",apiKey)
            print("Credentials?: ",credentials)
            print("WONDERFULL DATA?:\n",data)
        return service.postRelay(apiKey,data)
        #res = service.postRelay(apiKey,data)
        #code = 202
        #return successToResponse.getResponseWithMessage("MAJESTIC",code)
