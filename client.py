import socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


while 1:
    text = str(input())
    data = str.encode(text)
	client_socket.sendto(data, ('192.168.2.107',5060)) # 192.168.2.107 = RPi IP address, 6666 = Number Port
    print ("Sending request")

client_socket.close()
