import requests


class Servicio:
    URL1 = 'http://172.19.0.2:80/api'
    URL2 = 'http://192.168.1.53:5000/api'
    #URL1 = 'http://192.168.1.126:5000/api'
    #URL2 = 'http://172.19.0.2:80/api'

    @staticmethod
    def send_data(autor, nota):
        data = Servicio.objectify(autor, nota)
        res = {}
        res1 = requests.get(Servicio.URL1+"/status")
        res2 = requests.get(Servicio.URL2+"/status")
        print(str(res1))
        if res1.status_code != 200 and res2.status_code != 200:
            return {'id':-1}
        
        if res1.status_code == 200 and res2.status_code != 200:
            res = requests.post(Servicio.URL1+"/add", json=data)
        elif res2.status_code == 200 and res1.status_code != 200:
            res = requests.post(Servicio.URL2+"/add", json=data)
        else:
            cp1 = res1.json()
            cp2 = res2.json()
            if cp2['len'] < cp1['len']:
                res = requests.post(Servicio.URL2+"/add", json=data)
            elif cp1['len'] < cp2['len']:
                res = requests.post(Servicio.URL1+"/add", json=data)
            else:
                if cp2['ram'] < cp1['ram']:
                    res = requests.post(Servicio.URL2+"/add", json=data)
                elif cp1['ram'] < cp2['ram']:
                    res = requests.post(Servicio.URL1+"/add", json=data)
                else:
                    if cp2['cpu'] <= cp1['cpu']:
                        res = requests.post(Servicio.URL2+"/add", json=data)
                    else:
                        res = requests.post(Servicio.URL1+"/add", json=data)
        body = res.json()
        return body

    @staticmethod
    def objectify(autor, nota):
        return {
            'autor': autor,
            'nota':nota
        }