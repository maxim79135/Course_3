import psycopg2
conn = psycopg2.connect(dbname='demo',user='max',password='smdandsid1',host='localhost')
cursor = conn.cursor()
cursor.execute("select a.model, count(s.fare_conditions) "\
               "from bookings.aircrafts "\
               "a inner join bookings.seats s on "\
               "s.aircraft_code = a.aircraft_code "\
               "where s.fare_conditions = 'Economy' "\
               "group by a.model")
records = cursor.fetchall()
for v in records:
    print(v)
cursor.close()
conn.close()
