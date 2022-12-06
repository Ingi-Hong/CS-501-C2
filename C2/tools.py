import config
from psycopg2 import connect, sql

# Builds an insert query


# Returns a conn and a cursor
def load():
    # Connect to C2
    try:
        print("Connecting")
        conn = connect(dbname=config.database, user=config.username,
                       password=config.password, host=config.host, port=config.port, connect_timeout=20)
        print("Success? Should be 0 -> ", conn.closed)
        cursor = conn.cursor()
    except:
        print("failed to connect")

    print(f"Connection closed? {conn.closed}")
    return conn, cursor


def insertQueryBuilder(tableName, columns):

    q = sql.SQL("INSERT INTO {table}({column}) VALUES ({value})").format(
        table=sql.Identifier(tableName),
        column=sql.SQL(',').join([
            sql.Identifier(x) for x in columns
        ]),
        value=sql.SQL(',').join(sql.Placeholder() * len(columns))
    )

    return q


def insertQueryBuilder(tableName, columns, returnStringList):

    q = sql.SQL("INSERT INTO {table}({column}) VALUES ({value}) RETURNING {returnThis}").format(
        table=sql.Identifier(tableName),
        column=sql.SQL(',').join([
            sql.Identifier(x) for x in columns
        ]),
        value=sql.SQL(',').join(sql.Placeholder() * len(columns)),
        returnThis=sql.SQL(',').join([
            sql.Identifier(x) for x in returnStringList
        ]
        )
    )
    return q


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
        response = cursor.fetchall()
        cursor.close()
        conn.close()
        return response
    except Exception as error:
        print(f"error on execute insert query: {error}")


# Executes a select query


def executeSelectQuery(query):
    try:
        conn, cursor = load()
        print()
        print("executing: ")
        print(query)
        print()
        cursor.execute(query)
        conn.commit()
        returnThis = cursor.fetchall()
        cursor.close()
        conn.close()
        return returnThis
    except Exception as e:
        print(f"error on execute select query: {e}")
