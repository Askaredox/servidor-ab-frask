import pymongo


class Mongo:
    def __init__(self):
        client = pymongo.MongoClient("mongodb://172.19.0.3:27017/")['galleta']
        self.PUBLICACIONES = client['relleno']

    def get_pubs(self):
        ret = []
        for x in self.PUBLICACIONES.find():
            ret.append(self.__as_object(x))
        return ret

    def get_len(self):
        return {'len': self.PUBLICACIONES.count_documents({})}

    def __as_object(self, obj):
        return {
            'autor': obj['autor'],
            'nota': obj['nota']
        }

    def save_pub(self, obj):
        ret = self.PUBLICACIONES.insert_one(obj)
        return str(ret.inserted_id)

