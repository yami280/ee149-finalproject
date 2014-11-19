#!/usr/bin/env python
import web

#tree = ET.parse('user_data.xml')
#root = tree.getroot()

urls = (
    #'/door/(.*)', 'Door'
    '/(.*)', 'Door'
)

app = web.application(urls, globals())

class Door:

    def GET(self, request):
        """Handles GET requests at /door/.*
        Returns information about whether the door was open, and who
        opened the door.
        """
        status_file = open("door_status.txt", "r")
        status = status_file.read()
        status_file.close()
        status = status.split("\n")
        if len(status) > 0:
            if int(status[0]):
                return "The door is open, and has been opened by {0}.".format(status[1])
            else:
                return "The door is closed."


    def POST(self, request):
        """Handles POST requests at /door/.*
        Params for POST: userid=___&event=0|1
        Updates whether the door is open, and who opened the door.
        """
        data = web.data()
        data = data.split("&")
        status_dict = {}
        for d in data:
            status_dict[d[:d.find("=")]] = d[d.find("=")+1:]
        print status_dict
        if "event" in status_dict and int(status_dict["event"]) == 1:
            new_status = "1\n{0}".format(status_dict["userid"])
        else:
            new_status = "0\n"

        status_file = open("door_status.txt", "w")
        status_file.write(new_status)
        status_file.close()


if __name__ == "__main__":
    app.run()
