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



@app.route('/postmethod', methods = ['POST'])
def post_javascript_data():
    credentials = request.form["credentials"]
    if credentials != "BAJS":
        eCode = 401
        eMsg = "Not the correct credentials!"
        return errorsToResponse.getResponse(eMsg,eCode)
    print("Credentials? ",credentials)
    jsdata = request.form['data']
    print("Data?: ",jsdata)
    filename = post_to_file(jsdata)
    params = { 'filename' : filename }
    return jsonify(params)


def get_file_content():
    with open(datafile, "r") as myfile:
        print("FILE CONTENT: ",myfile.readlines)
def post_to_file(jsdata):
    with open(datafile, "a") as myfile:
        myfile.write("text appended\n")
    print(jsdata)
    return "FAKE FUCKER!"