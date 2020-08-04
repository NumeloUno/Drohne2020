import socket
import serial

ser = serial.Serial('/dev/ttyACM0',9600)

# Create a Server Socket and wait for a client to connect
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(('', 5060))
print ("UDPServer Waiting for client on port 6666")

# Define moving functions

# Recive data from client and decide which function to call
while True:
    dataFromClient, address = server_socket.recvfrom(256)
    if(dataFromClient != ""):
        print(dataFromClient)
        ser.write(dataFromClient)
