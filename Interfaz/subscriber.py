import paho.mqtt.client as mqtt
import psycopg2

conn = psycopg2.connect(database="feeding_db",
                            user="postgres",
                            password="tty123",
                            host="localhost",
                            port="5432")

cursor = conn.cursor()


def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("feeding-data")

def on_message(client, userdata, message):
    print(f"Received message: {message.payload.decode()} on topic {message.topic}")
    msg = message.payload.decode().split('$')
    peso1, peso2 = msg[0], msg[1]
    try:
        cursor.execute("INSERT INTO feeding_data (comida, agua) VALUES (%s, %s);",
            (peso1, peso2))
    except (Exception, psycopg2.Error) as error:
            print(error.pgerror)
    conn.commit()


# Create a new MQTT client instance
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("mqtt.eclipseprojects.io", 1883, 60)

client.loop_forever()