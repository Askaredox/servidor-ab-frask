from flask import Flask, request
from flask_cors import CORS
from readmod import ReadMod
from mongo import Mongo
import json
import random


app = Flask(__name__)
cors = CORS(app, resources={r"/api/*": {"origin": "*"}})
db = Mongo()


@app.route('/')
def check():
    return "Todo good"


@app.route('/api/pubs', methods=['GET'])
def pubs():
    return json.dumps(db.get_pubs())


@app.route('/api/status', methods=['GET'])
def status():
    ram_cpu = ReadMod.get_vals()
    db_len = db.get_len()
    ret = {**ram_cpu, **db_len}
    return json.dumps(ret)


@app.route('/api/add', methods=['POST'])
def add():
    if request.method == 'POST':
        content = request.get_json()
        autor = content['autor']
        nota = content['nota']

        obj = {
            'autor': autor,
            'nota': nota
        }
        ret = db.save_pub(obj)
        return {"id": ret}


@app.route('/api/watch', methods=['GET'])
def watch():
    status = ReadMod.get_vals()
    return json.dumps(status)


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
