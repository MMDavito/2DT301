from flask_restful import Resource,request
from service.relay_service import RelayService
from support.Response_Maker import IS_DEBUG,errorsToResponse,successToResponse
service = RelayService()

class RelayController(Resource):
    def get(self):
        if IS_DEBUG:
            print("ARDUINO FUCK YES!")
            print("FORMBAJS: ",request.headers)
        apiKey = request.headers["x-api-key"]
    
        credentials = request.headers["Credentials"]
        if IS_DEBUG:
            print("Api key?: ",apiKey)
            print("Credentials?: ",credentials)

        if apiKey != "1337" or credentials != "BAJS":
            eCode = 401
            eMsg = "Not the correct credentials!"
            return errorsToResponse.getResponse(eMsg,eCode)

        #ELSE:
        return service.getRelay(apiKey)
    def post(self):
        if IS_DEBUG:
            print("ARDUINO FUCK YES!")
            print("FORMBAJS: ",request.headers)
        
        apiKey = request.headers["x-api-key"]
        credentials = request.headers["Credentials"]
        if IS_DEBUG:
            print("Api key?: ",apiKey)
            print("Credentials?: ",credentials)

        if apiKey != "1337" or credentials != "BAJS":
            eCode = 401
            eMsg = "Not the correct credentials!"
            return errorsToResponse.getResponse(eMsg,eCode)
        data = request.json
        if data == None:
            eCode = 400
            eMsg = "Incorrect formatt/Bad request!"
            return errorsToResponse.getResponse(eMsg,eCode)
        #ELSE:
        if IS_DEBUG:
            print("Api key?: ",apiKey)
            print("Credentials?: ",credentials)
            print("WONDERFULL DATA?:\n",data)
        service.postRelay(apiKey,data)
        return "MAGNIFIC"
