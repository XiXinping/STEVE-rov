import socket
import time

buf_size = 0x400 #buffer size, if changed set to power of 2
port = 2000 #semi-arbitrary number, can be set to any valid port not in use
request = "test"

# Creating and binding sockets
s = socket.socket() #creating socket
print(s.connect(("0.0.0.0", port)))
while True:
    time.sleep(1)
    s.sendall(request.encode())

