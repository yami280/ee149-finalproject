#!/usr/bin/env python
import web
import os
import json

DIRECTORY = "server/"

urls = (
    #'/door/(.*)', 'Door'
    '/(.*)', 'Server'
)

class Server:

    def GET(self, request):
        query = web.input()
        if len(query) > 0: 
            if "sensor" in query and "value" in query and "id" in query:
                status_file = open(DIRECTORY+query["sensor"]+".txt", "a+")
                status_file.seek(0, os.SEEK_SET)
                try:
                    json_data = json.load(status_file)
                except ValueError:
                    json_data = {}
                status_file.close()
                json_data[query["id"]] = query["value"]
                status_file = open(DIRECTORY+query["sensor"]+".txt", "w")
                json.dump(json_data, status_file)
                status_file.close()
                return query["id"]
        else:
            path = web.ctx.fullpath[1:]
            path = path.split("/")
            print path
            if len(path) > 1 and path[1] != "":
                idx = path[1]
            else:
                idx = None
            if os.path.isfile(DIRECTORY+path[0]+".txt"):
                status_file = open(DIRECTORY+path[0]+".txt", "r")
                json_data = json.load(status_file)
                status_file.close()
                if idx == None:
                    return json_data
                else:
                    return json_data[idx]
                

class MyApplication(web.application):
    def run(self, port=8080, *middleware):
        func = self.wsgifunc(*middleware)
        return web.httpserver.runsimple(func, ('0.0.0.0', port))

app = MyApplication(urls, globals())
if __name__ == "__main__":
    heroku_port = int(os.environ.get('PORT', 8080))
    app.run(port=heroku_port)
