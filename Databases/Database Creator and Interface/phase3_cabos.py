import sqlite3
import datetime
from pathlib import Path


def userselect():
    x = '-1'
    while x != '0':
        print('\n--------------------------------------' + \
              '\nSelect from one of the following options:' + \
              '\n--------------------------------------')
        x = input("\n1) Retrieve all data\n2) Average\n3) Insert\n4) Delete\n0) Exit\n\nEnter Selection: ")

        if x == '0':
            print("\nTerminating application...\n\n")
            break
        elif x == '1':
            retrieve_all()
        elif x == '2':
            get_average()
        elif x == '3':
            insert()
        elif x == '4':
            delete()
        else:
            print("Error: Enter valid input")
        
                
def retrieve_all():
    chosen_table = input("\nEnter the name of the table to access: ")
    query = 'select * from ' + chosen_table
    cursor.execute(query)
    result = cursor.fetchall()
    for x in result:
        print(x)


def get_average():
    chosen_table = input("\nEnter the name of the table to access: ")
    chosen_column = input("Enter the name of the column: ")
    query = 'select avg(' + chosen_column + ') from ' + chosen_table
    cursor.execute(query)
    print('\nAverage of ' + chosen_column + ': ' + "%.2f" % round(cursor.fetchone()[0], 2))


def insert():
    chosen_table = input("\nEnter the name of the table to access: ")
    query = 'select * from ' + chosen_table
    cursor.execute(query)
    result = [d[0] for d in cursor.description]

    new_entry = []
    query = 'insert into \'' + chosen_table + '\' values (' 
    print("Enter the following parameters to add to " + chosen_table + ":")
    for i in range(0, len(result)):
        new_entry.append(input(result[i] + ": "))
        query += "?"
        if i != len(result) - 1:
            query += ", "
    query += ")"
    cursor.execute(query, new_entry)



def delete():
    chosen_table = input("\nEnter the name of the table to access: ")
    query = 'select * from ' + chosen_table
    cursor.execute(query)
    columns = [d[0] for d in cursor.description]
    print(columns)
    result = cursor.fetchall()

    data_types = []
    for x in result[0]:
        data_types.append(type(x))
    for x in result:
        print(x)

    entry_delete = []
    print('Enter the parameters for the entry to be deleted:')
    for i in range(0, len(columns)):
        param = input(columns[i] + ': ')
        if data_types[i] == int:
            entry_delete.append(int(param))
        elif data_types[i] == float:
            entry_delete.append(float(param))
        elif data_types[i] == datetime.date:
            entry_delete.append(datetime.date(param))
        elif data_types[i] == datetime.datetime:
            entry_delete.append(datetime.datetime(param))
        else:
            entry_delete.append(param)

    query = 'delete from \'' + chosen_table + '\' where '
    for i in range(0, len(columns)):
        query += columns[i] + " = ?"
        if i != len(columns) - 1:
            query += " and "
        else:
            query += ";"
    
    cursor.execute(query, entry_delete)
    result = cursor.fetchall()
    for x in result:
        print(x)



if __name__ == "__main__":
    conn = sqlite3.connect('hospital.db')
    cursor = conn.cursor()
    userselect()
    conn.commit()
    conn.close()