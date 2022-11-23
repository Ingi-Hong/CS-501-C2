# The C2 Server 
# Make sure you have the .env file msg 

from dotenv import load_dotenv, find_dotenv
import os
import psycopg2
from flask import Flask, request, jsonify 
import random 

app = Flask(__name__)

# Load in .env variables to connect to PostgreSQL server 
load_dotenv(find_dotenv())
host = os.getenv('host')
password = os.getenv('password')
username = os.getenv('username')
port = os.getenv('port')
database = os.getenv('database')
sslmode = os.getenv('require')

#Connect to C2
print("Connecting")
conn = psycopg2.connect(f"dbname={database} user={username} password={password} host={host} port={port}")
print("Success? Should be 0: ", conn.closed)
cursor = conn.cursor()

# For when you want to any query, used to avoid connections timing out 
def executeQuery(query):
    cursor.execute(query)
    returnThis = cursor.fetchall()

    return returnThis 

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