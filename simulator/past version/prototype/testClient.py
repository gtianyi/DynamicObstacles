import socket
import sys
import time
# Create a TCP/IP socket

sock = 0
# Connect the socket to the port where the server is listening
server_address = ('localhost', 3000)
print ( 'connecting to %s port %s' % server_address)


messages = [b'start',b'2', b'1',b'2',b'1',b'1',b'2', b'1',b'2',b'1',b'1']
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(server_address)
for i in range(len(messages)):

    try:
        # Send data
        # message = b'This is the message.  It will be repeated.'
        message = messages[i]
        print ('sending "%s"' % message)
        sock.sendall(message)

        # Look for the response
        # amount_received = 0
        # amount_expected = len(message)
        #
        # while amount_received < amount_expected:
        data = sock.recv(2048)
        time.sleep(0.5)
            # amount_received += len(data)
        print ('received "%s"' % data)
    # finally:
    #     pass
    except:
        pass


#     finally:
# # print ('closing socket')
#         sock.close()
