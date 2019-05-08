import http.server
import socketserver
from http import HTTPStatus
import sys
import json
#import Adafruit_DHT

data_transmission = False

class Handler(http.server.SimpleHTTPRequestHandler):
	def _set_headers(self):
		self.send_response(200)
		self.send_header('Content-type', 'application/json')
		self.end_headers()

	def do_GET(self):
		global data_transmission
		if data_transmission == True:
			self.send_response(200)
			self.send_header('Content-type', 'text/plain')
			self.end_headers()
			#hu, te = Adafruit_DHT.read_retry(11, 4)
			te = '21'
			mystr = str(te)
			self.wfile.write(mystr.encode(encoding='utf_8'))
		else:
			self.send_response(HTTPStatus.OK)
			self.end_headers()
			#self.wfile.write(b'Turn on')

	def do_POST(self):
		global data_transmission
		data_transmission = not data_transmission
		print("DATA TRANSMISSION: ", data_transmission)
		self._set_headers()
		self.wfile.write(bytes([data_transmission]))


httpd = socketserver.TCPServer(('192.168.1.193', 8000), Handler)
httpd.serve_forever()
