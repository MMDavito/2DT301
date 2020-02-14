from flask import Response
from datetime import datetime
import json
"""
This is not only global variables, it is mostely helper/support functions
But IS_DEBUG is still here
"""
IS_DEBUG = True
#IS_DEBUG = False
class timeHelper:
    def strToDate(date:str):
        date_form = '%Y-%m-%d'
        return datetime.strptime(date,date_form).date()
    def strToDateTime(date_time:str):
        date_time_form = '%Y-%m-%d %H:%M:%S' 
        return datetime.strptime(date_time,date_time_form)
    def getUtcDateTime():
        date_time_form = '%Y-%m-%d %H:%M:%S' 
        return datetime.utcnow().strftime(date_time_form)
    def dateTimeToStr(dT:datetime):
        format_datetime = "%Y-%m-%d %H:%M:%S"
        if dT != None:
            dT = dT.strftime(format_datetime)#As I recall it arguments are copies unless self.somethingSomething
        return dT

"""If I ever refactor this is good, but right now i need to create objects of them.
class dictHelper:
    def(data:dict,keys:tuple,new_keys:tuple)
"""
class errorsToResponse:
    """
    def __init__(self):
        errorsToResponse.form = {
            "msg":None
            "data":None
        }
    """ 
    def get_form(msg,data):
        form = {
            "error":{
            "msg":msg,
            "data":data
            }
        }
        return form

    def getResponse(msg:str,error_code:int):
        """
        Returns a Flask.Response
        """
        form = errorsToResponse.get_form(msg,None)
        print("HLLO FREIND")
        temp_resp = Response((json.dumps(form)), status=error_code, mimetype='application/json')

        return temp_resp


    def getResponseWithAll(msg:str,data:dict,error_code:int):
        """
        Returns a Flask.Response
        """
        print("HLLO FREIND")
        form = errorsToResponse.get_form(msg,data)
        temp_resp = Response((json.dumps(form)), status=error_code, mimetype='application/json')
        return temp_resp


class successToResponse:
    def get_form(msg,data):
        form = {
            "msg":msg,
            "data":data
        }
        return form

    def getResponse(code:int):
        temp_resp = Response(status=code)

    def getResponseWithMessage(msg:str,code:int):
        """
        Returns a Flask.Response
        """
        form = successToResponse.get_form(msg,None)
        print("HLLO FREIND")
        temp_resp = Response((json.dumps(form)), status=code, mimetype='application/json')

        return temp_resp
    def getResponseWithData(data:dict,code:int):
        """
        Returns a Flask.Response
        """
        print("HLLO FREIND")
        form = successToResponse.get_form("",data)
        temp_resp = Response((json.dumps(form)), status=code, mimetype='application/json')
        print(form)
        print("RESPONSEFAN: ")
        print(temp_resp)
        return temp_resp

    def getResponseWithAll(msg:str,data:dict,code:int):
        """
        Returns a Flask.Response
        """
        print("HLLO FREIND")
        form = successToResponse.get_form(msg,data)
        temp_resp = Response((json.dumps(form)), status=code, mimetype='application/json')
        return temp_resp
    
