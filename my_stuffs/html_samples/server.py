from RangeHTTPServer import RangeRequestHandler
import http.server as SimpleHTTPServer
import argparse
from subprocess import Popen, PIPE

class MyServerHandler(RangeRequestHandler):
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        process = Popen(['./gen_payload', 'payload', post_data], stdout=PIPE)
        stdout = process.communicate()
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        self.wfile.write(stdout[0])

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('port', action='store', default=8000, type=int, nargs='?', help='Specify alternate port [default: 8000]')
    args = parser.parse_args()

    SimpleHTTPServer.test(HandlerClass=MyServerHandler, port=args.port)
