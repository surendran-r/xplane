import socket

byte_message = bytes("CMND0sim/flight_controls/flaps_up", "ascii")
opened_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sent = opened_socket.sendto(byte_message, ("localhost", 49009))
print(sent)