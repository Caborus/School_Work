import sqlite3
import os
import sys
from pathlib import Path


def createDB(conn):
    with open('create_db.sql', 'r') as f:
        create_db = f.read()
    with open('insert_db.sql', 'r') as f:
        insert_db = f.read()

    conn.executescript(create_db)
    conn.executescript(insert_db)



if __name__ == "__main__":
    user_input = sys.argv[1:]
    
    path = Path('./hospital.db')

    # connect to the database
    if(path.is_file() == False):
        conn = sqlite3.connect('hospital.db')
        createDB(conn)
    elif(os.path.getsize('./hospital.db') == 0):
        conn = sqlite3.connect('hospital.db')
        createDB(conn)
    else:
        conn = sqlite3.connect('hospital.db')

    # close the database connection
    conn.commit()
    conn.close()