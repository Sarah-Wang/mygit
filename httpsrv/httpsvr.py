#!/usr/bin/python
import SimpleHTTPServer
import SocketServer
import sys

def start():
    host = ""
    port = 1234
    if len(sys.argv) > 1:
        arg = sys.argv[1]
        if ':' in arg:
            host, port = arg.split(':')
            port = int(port)
        else:
            try:
                port = int(sys.argv[1])
            except:
                host = sys.argv[1]
    Handler = SimpleHTTPServer.SimpleHTTPRequestHandler
    httpd = SocketServer.TCPServer((host, port), Handler)
    print "serving at host: ", host
    print "serving at port: ", port
    httpd.serve_forever()

if __name__ == "__main__":
    start()
