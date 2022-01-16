from importlib.machinery import SourceFileLoader
import socket
import arduino_functions

buf_size = 0x400 #buffer size, if changed set to power of 2
port = 2000 #semi-arbitrary number, can be set to any valid port not in use

# Creating and binding sockets
s = socket.socket() #creating socket
s.bind(('', port)) #binding to port
s.listen(1) #allowing 1 clients to connect
print("listening")

#Starting a while loop that will except connections
c, addr = s.accept()
print("Connection from", addr)
c.send("You have connected\n".encode()) #sending greeting message

def interpret_data(data):
    if data == "test":
        arduino_functions.test()
    elif data == "blink":
        arduino_functions.blink()
    elif data == "water":
        arduino_functions.water()

while True:
    data = c.recv(buf_size)
    interpret_data(data.decode())
    if not data:
        break
    
