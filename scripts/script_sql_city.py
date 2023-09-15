import psycopg2

conn = psycopg2.connect(user = "astra", password = "12345678", host = "127.0.0.1")
cursor = conn.cursor()

conn.autocommit = True

s = "DROP DATABASE forcity;"
sql = "CREATE DATABASE forcity;"

cursor.execute(s)
cursor.execute(sql)

conn.commit();

#####################################

conn = psycopg2.connect(dbname = "forcity", user = "astra", password = "12345678", host = "127.0.0.1")
cursor = conn.cursor()

conn.autocommit = True

cursor.execute(open('/home/astra/DATABASE_MVC/script_sql_city.sql', 'r').read())

conn.commit();
cursor.close()
conn.close()
