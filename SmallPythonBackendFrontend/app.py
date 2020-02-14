from __future__ import print_function
from flask import Flask, render_template, make_response,Response
from flask import redirect, request, jsonify, url_for
import os
from support.Response_Maker import errorsToResponse,successToResponse
app = Flask(__name__)
app.secret_key = 's3cr3t'
app.debug = True
app._static_folder = os.path.abspath("templates/static/")
datafile = "data/data.txt"
arduino_datafile = "data/arduino_data.txt"
fileLength = 10

@app.route('/temp/', methods=['GET'])
def temp():
    title = 'Create the input'
    return render_template('layouts/temp.html',
                           title=title)

@app.route('/', methods=['GET'])
def index():
    title = 'Create the input'
    return render_template('layouts/index.html',
                           title=title)

@app.route('/results/', methods=['GET'])
def results():
    title = 'Result'
    data = get_file_content()

    return render_template('layouts/results.html',
                           title=title,
                           data=data)
@app.route('/arduino/', methods=['GET'])
def arduino():
    title = 'Arduino Result'
    return render_template('layouts/arduino.html',
                           title=title)

@app.route('/results/<fName>', methods=['GET'])
def results_named(fName):#FileName
    title = 'Result'
    #data = get_file_content(fName)
    data = get_file_content()

    return render_template('layouts/results.html',
                           title=title,
                           data=data)


@app.route('/postmethod', methods = ['POST'])
def post_javascript_data():
    credentials = request.form["Credentials"]
    print("Credentials? ",credentials)
    jsdata = request.form['data']
    print("Data?: ",jsdata)

    if credentials != "BAJS":
        eCode = 401
        eMsg = "Not the correct credentials!"
        return errorsToResponse.getResponse(eMsg,eCode)
    filename = post_to_file(jsdata)
    print("FILENAME: ",filename)
    params = { 'filename' : filename }
    return jsonify(params)
@app.route('/shitty_human_data',methods=['GET'])
def get_shitty_human_data():
    print("ARDUINO FUCK YES!")
    print("FORMBAJS: ",request.headers)
    credentials = request.headers["Credentials"]
    print("Credentials? ",credentials)

    if credentials != "BAJS":
        eCode = 401
        eMsg = "Not the correct credentials!"
        return errorsToResponse.getResponse(eMsg,eCode)
    data = get_file_content()
    print("DATA: ",data)
    return successToResponse.getResponseWithData(data,200)

@app.route('/arduino_data',methods=['GET'])
def get_arduino_data():
    print("ARDUINO FUCK YES!")
    print("FORMBAJS: ",request.headers)
    credentials = request.headers["Credentials"]
    print("Credentials? ",credentials)

    if credentials != "BAJS":
        eCode = 401
        eMsg = "Not the correct credentials!"
        return errorsToResponse.getResponse(eMsg,eCode)
    data = get_arduiono_data_file()
    print("DATA: ",data)
    return successToResponse.getResponseWithData(data,200)
@app.route('/arduino_data',methods=['POST'])
def post_arduino_data():
    print("ARDUINO FUCK YES!")
    print("FORMBAJS: ",request.headers)
    credentials = request.headers["Credentials"]
    print("Credentials? ",credentials)
    
    if credentials != "BAJS":
        eCode = 401
        eMsg = "Not the correct credentials!"
        return errorsToResponse.getResponse(eMsg,eCode)
    data = get_file_content()
    print("DATA: ",data)
    return successToResponse.getResponseWithData(data,200)


def get_file_content():
    with open(datafile, "r") as myfile:
        lines = myfile.readlines()
        print("FileCONTENT: ",lines)
        print("FILE CONTENT: ",myfile.readlines)
    return lines

def get_arduiono_data_file():
    """
    Returns list of data (line by line) from the datafile.
    """
    with open(arduino_datafile, "r") as myfile:
        lines = myfile.readlines()
        print("arduinoFileContents: ",lines)
        print("FILE CONTENT: ",myfile.readlines)
    return lines

def post_to_arduinofile(jsdata,credentials):
    if credentials != "ARDUINO_BAJS":
        eCode = 401
        eMsg = "Not the correct credentials!"
        return errorsToResponse.getResponse(eMsg,eCode)
    #ELSE:        
    print("JSDATA: ",jsdata)
    lines = get_arduiono_data_file()
    if len(lines) < fileLength:
        with open(arduino_datafile, "a") as myfile:
            myfile.write(jsdata+"\n")
    else:
        with open(arduino_datafile, "w") as myfile:
            for i in range(1,fileLength):
                myfile.write(lines[i])
            myfile.write(jsdata+"\n")
    return "FAKE_FUCKER!"


def post_to_file(jsdata):
    print("JSDATA: ",jsdata)
    lines = get_file_content()
    if len(lines) < fileLength:
        with open(datafile, "a") as myfile:
            myfile.write(jsdata+"\n")
    else:
        with open(datafile, "w") as myfile:
            for i in range(1,fileLength):
                myfile.write(lines[i])
            myfile.write(jsdata+"\n")
    return "FAKE_FUCKER!"