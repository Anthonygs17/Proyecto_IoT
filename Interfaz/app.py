from flask import Flask, request, render_template, Response
import psycopg2
import json

app = Flask(__name__)

conn = psycopg2.connect(
    database="feeding_db",
    user="postgres",
    password="tty123",
    host="localhost",
    port="5432")

cursor = conn.cursor()


@app.route('/static/<content>')
def static_content(content):
    return render_template(content)

@app.route('/', methods = ['GET'])
def get_index():
    return render_template('index.html')


@app.route('/feeding-data', methods = ['GET'])
def get_data():
    query = f"select comida, agua from feeding_data order by id desc limit 1;"
    cursor.execute(query)
    res = cursor.fetchone()
    if res is None:
        msg = {"error": "Recurso no encontrado"}
        return Response(json.dumps(msg), status=404, mimetype='application/json')
    r_msg = {'comida': str(res[0]), 'agua': str(res[1])}
    json_msg = json.dumps(r_msg)
    return Response(json_msg, status=200, mimetype='application/json')


if __name__ == "__main__":
    app.run(debug=False, port=5000)