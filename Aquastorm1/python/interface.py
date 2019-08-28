from threading import RLock
from threading import Lock
import numpy as np
import logging
import config
import base64
import struct
import time
import zmq
import cv2

log = logging.getLogger(__name__)


class Interface(object):
    def __init__(self):
        self.sub = zmq.Context()
        if self.sub is not None:
            self.sub = self.sub.socket(zmq.REQ)
            if self.sub is not None:
                self.sub.connect("tcp://localhost:5554")
                log.debug("connected to the zmq server!")

        self.lock = RLock()
        log.debug("created interface object")

    def __del__(self):
        self.sub.close(linger=None)

    def get_image(self, cam_id):
        self.lock.acquire()
        log.debug("get_image lock acquired at {}".format(time.time()))
        if cam_id == config.F_IDX:
            self.sub.send_string("igf")
        else:
            self.sub.send_string("igd")
        image = base64.b64decode(self.sub.recv())
        self.lock.release()
        return image

    def set_image(self, cam_id, img):
        self.lock.acquire()
        log.debug("set_image lock acquired at {}".format(time.time()))
        _, buf = cv2.imencode(".jpg", img, [int(cv2.IMWRITE_JPEG_QUALITY), 90])
        buf_encoded = base64.b64encode(buf)
        if cam_id == 0:
            self.sub.send_string("isf")
        else:
            self.sub.send_string("isd")
        self.sub.recv()
        self.sub.send(buf_encoded)
        self.sub.recv()
        self.lock.release()
        log.debug("set_image lock released at {}".format(time.time()))

    def get_current_state(self):
        with self.lock:
            log.debug("get_state acquired lock at {}".format(time.time()))
            self.sub.send_string("sgc")
            response = self.sub.recv().decode("utf-8")
            return [float(i) for i in response.split(" ")[1:]]

    def get_desired_state(self):
        with self.lock:
            log.debug("get_state acquired lock at {}".format(time.time()))
            self.sub.send_string("sgd")
            response = self.sub.recv().decode("utf-8")
            return [float(i) for i in response.split(" ")[1:]]
    
    def set_current_state(self, state):
        with self.lock:
            log.debug("set_state acquired lock at {}".format(time.time()))
            self.sub.send_string("ssc {} {} {} {} {} {}".format(*state))
            response = self.sub.recv().decode("utf-8")
            if response == "s ok":
                log.debug("Set state.")
            else:
                log.error("Sub did not acknowledge state set.")  

    def set_desired_state(self, state):
        with self.lock:
            log.debug("set_state acquired lock at {}".format(time.time()))
            self.sub.send_string("ssd {} {} {} {} {} {}".format(*state))
            response = self.sub.recv().decode("utf-8")
            if response == "s ok":
                log.debug("Set state.")
            else:
                log.error("Sub did not acknowledge state set.")  

    def get_kill(self):
        with self.lock:
            log.debug("get_kill acquired lock at {}".format(time.time()))
            self.sub.send_string("kg")
            response = self.sub.recv().decode("utf-8")
            if response.startswith("k "):
                return response.split(" ")[1] == 1  # returns True if alive, False if dead
            log.error("get_kill couldn't parse kill, releasing lock at {}".format(time.time()))

    def set_kill(self, kill_state):  # pass False to kill sub, pass True to unkill.
        with self.lock:
            log.debug("set_kill acquired lock at {}".format(time.time()))
            self.sub.send_string("ks {}".format(1 if kill_state else 0))
            response = self.sub.recv().decode("utf-8")
            if response == "k ok":
                if kill_state:
                    log.debug("Killed sub.")
                else:
                    log.debug("Unkilled sub.")

    def get_logs(self):
        with self.lock:
            log.debug("get_logs acquired lock at {}".format(time.time()))
            self.sub.send_string("l")
            response = self.sub.recv().decode("utf-8")
            return response.split("\n")

