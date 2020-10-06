from flask import Flask, request
from flask_cors import CORS
from servicio import Servicio
import json

app = Flask(__name__)
cors = CORS(app, resources={r"/api/*": {"origin": "*"}})


@app.route('/')
def check():
    return "Todo good en Servidor 1"

@app.route('/api/add', methods=['POST'])
def add():
    if request.method == 'POST':
        content = request.get_json()
        autor = content['autor']
        nota = content['nota']

        ret = Servicio.send_data(autor, nota)
        return json.dumps(ret)
        


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
