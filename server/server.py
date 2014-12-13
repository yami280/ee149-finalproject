#!/usr/bin/env python
import web
import os

DIRECTORY = "server/"

urls = (
    #'/door/(.*)', 'Door'
    '/(.*)', 'Server'
)

class Server:

    def GET(self, request):
        """Handles GET requests.
        """
        query = web.input()
        if len(query) > 0: 
            if "sensor" in query and "value" in query:
                status_file = open(DIRECTORY+query["sensor"]+".txt", "a+")
                status_file.write(query["value"]+"\n")
                status_file.seek(0, os.SEEK_SET)
                ret_id = len(status_file.read().split("\n")) - 1
                status_file.close()
                return ret_id
        else:
            path = web.ctx.fullpath[1:]
            path = path.split("/")
            print path
            if len(path) > 1 and path[1] != "":
                idx = int(path[1])-1
            else:
                idx = None
            if os.path.isfile(DIRECTORY+path[0]+".txt"):
                status_file = open(DIRECTORY+path[0]+".txt", "r")
                lines = status_file.read().split("\n")
                status_file.close()
                lines.pop()
                if idx == None:
                    return lines
                elif idx > len(lines):
                    return
                else:
                    return lines[idx]
                

class MyApplication(web.application):
    def run(self, port=8080, *middleware):
        func = self.wsgifunc(*middleware)
        return web.httpserver.runsimple(func, ('0.0.0.0', port))

app = MyApplication(urls, globals())
if __name__ == "__main__":
    heroku_port = int(os.environ.get('PORT', 8080))
    app.run(port=heroku_port)
