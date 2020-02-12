from __future__ import print_function
from flask import Flask, render_template, make_response
from flask import redirect, request, jsonify, url_for
import os

app = Flask(__name__)
app.secret_key = 's3cr3t'
app.debug = True
app._static_folder = os.path.abspath("templates/static/")

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
    jsdata = request.form['canvas_data']
    unique_id = create_csv(jsdata)
    params = { 'uuid' : unique_id }
    return jsonify(params)

def get_file_content():
    return "BLAHA"

def create_csv(jsdata):
    print(jsdata)
    return "FAKE FUCKER!"