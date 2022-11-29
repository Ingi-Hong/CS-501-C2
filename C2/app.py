# The C2 Server 

import os

import psycopg2
from flask import Flask, jsonify, request

app = Flask(__name__)

print(os.environ)
host = os.environ['host']
password = os.environ['password']
username = os.environ['username']
port = os.environ['thePort']
database = os.environ['database']
def load():
    # Connect to C2
    try:
        print("Connecting")
        conn = psycopg2.connect(dbname=database, user=username, password=password, host=host, port=port, connect_timeout=10)
        print("Success? Should be 0: ", conn.closed)
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

@app.route("/hello", methods=["GET"])
def sure():
    try:
        cursor = load()
    except:
        print("failure")
    print(host, password, username, port, database)
    return "<p>hey!!</p>"

@app.route("/whoami", methods=["GET"])
def handle_whoami():
    #TO IMPLEMENT 
    pass 

@app.route("/test", methods=["GET"])
def handle_test():
    stuff = executeQuery("SELECT * from test")
    return ("<p>%s</p>" % stuff)

@app.route("/")
def hello():
    return "<p>Hello, World!</p>"
