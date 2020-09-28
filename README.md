# Flask/Mongo

## Flask API

ruta: 

| Ruta        | Método | Desc.                                | Devuelve             |
| ----------- | ------ | ------------------------------------ | -------------------- |
| /           | GET    | Heath Check                          | "Todo good"          |
| /api/pubs   | GET    | obtener todas las publicaciones      | [{autor,nota},{}...] |
| /api/status | GET    | Obtener RAM, CPU y cantidad de datos | {ram,cpu,len}        |
| /api/add    | POST   | Añadir un nuevo dato                 | {id}                 |
| /api/watch  | GET    | Obtener RAM y CPU                    | {ram,cpu}            |

## Mongo

Database: "galleta"

Collection: "relleno"