import numpy as np
import time 
import zmq 
import base64 
import cv2 

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

while True:
    message = socket.recv()
    image = base64.b64decode(message)
    npimg = np.fromstring(image, dtype=np.uint8)
    source = cv2.imdecode(npimg, 1)
    # cv2.imshow("image", source)
    # cv2.waitKey(1)
    socket.send(b"World")
