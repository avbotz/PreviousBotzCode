from threading import Thread
import time
import logging
import base64
import sys
import cv2

import config

"""Functions that handle events going to and coming from the website"""

log = logging.getLogger(__name__)


class CameraThread(Thread):
    def __init__(self, interface, socket):
        Thread.__init__(self)
        self.frame_counter = 0
        self.fps1 = time.time()
        self.fps2 = time.time()
        self.socket = socket
        self.interface = interface
        self.setDaemon(False)
        time.sleep(.5)

    def image_fetch_func(self):
        while True:
            log.debug("About to get images, hold my beer...")
            front_img = self.interface.get_image(config.F_IDX)
            if front_img is not None:
                self.socket.emit('front_img', front_img, broadcast=True)
                self.socket.sleep(0)  # Needed to allow other threads to execute, apparently
                log.debug("Sent front image to website!")
            down_img = self.interface.get_image(config.D_IDX)
            if down_img is not None:
                self.socket.emit('down_img', down_img, broadcast=True)
                self.socket.sleep(0)
                log.debug("Sent down image to website!")
            self.debug_img_fetch_func()
            self.frame_counter += 1
            time.sleep(1. / config.FRAMERATE)
            if config.LOG_FPS:
                if self.frame_counter >= config.FRAMERATE:
                    self.fps2 = time.time()
                    log.info("fps: {}".format(str(int(self.frame_counter / (self.fps2 - self.fps1)))))
                    self.frame_counter = 0
                    self.fps1 = self.fps2
    
    def debug_img_fetch_func(self):
        img = cv2.imread("logs/debug_img.png", 1)
        if img is not None:
            jpeg = cv2.imencode(".jpg", img)[1]
            base64jpeg = base64.b64encode(jpeg)
            self.socket.emit('debug_img', base64jpeg, broadcast=True)
            self.socket.sleep(0)
            log.debug("Sent debug image to website!")
        else:
            log.error("Failed to read debug image.")


    def run(self):
        try:
            self.image_fetch_func()
        except KeyboardInterrupt:
            sys.exit()


class StateThread(Thread):
    def __init__(self, interface, socket):
        Thread.__init__(self)
        self.socket = socket
        self.interface = interface
        self.setDaemon(False)
        time.sleep(.5)
        @self.socket.on('state_set')
        def set_state(json):
            self.interface.set_desired_state(json)
            log.info("Received state {} from website, sending to Aquastorm.".format(json))

        @self.socket.on('kill_set')
        def set_kill(json):
            self.interface.set_kill(json)
            log.info("Received kill state {} from website, sending to Aquastorm.".format(json))

        @socket.on("manual_control")
        def manual_control(json):
            state = self.interface.get_current_state()
            xmotion = state[0]
            if (json["mforward"]):
                xmotion += 1000
            if (json["mback"]):
                xmotion -= 1000
            ymotion = state[1]
            if (json["mright"]):
                ymotion += 1000
            if (json["mleft"]):
                ymotion -= 1000
            yaw = state[3]
            if (json["rright"]):
                yaw += 1000
            if (json["rleft"]):
                yaw -= 1000
            pitch = state[4]
            if (json["pforward"]):
                pitch += 1000
            if (json["pback"]):
                pitch -= 1000
            new_state = [xmotion, ymotion, state[2], yaw, pitch, state[5]]
            print(new_state)
            self.interface.set_desired_state(new_state)


    def state_fetch_func(self):
        while True:
            latest_state = self.interface.get_current_state()
            latest_kill = self.interface.get_kill()
            log.info("Got state {}".format(latest_state))
            self.socket.emit('state_get', latest_state, broadcast=True)
            self.socket.emit('kill_get', latest_kill, broadcast=True)
            self.socket.sleep(0)
            log.debug("state sent to website!")
            time.sleep(1. / config.STATERATE)

    def run(self):
        try:
            self.state_fetch_func()
        except KeyboardInterrupt:
            sys.exit()

class LogThread(Thread):
    def __init__(self, interface, socket):
        Thread.__init__(self)
        self.interface = interface
        self.socket = socket
        self.setDaemon(False)
    
    def log_fetch_func(self):
        while True:
            logs = self.interface.get_logs()
            self.socket.emit('logs_get', logs, broadcast=True)
            self.socket.sleep(0)
            log.debug("logs sent to website!")
            time.sleep(1. / config.LOGRATE)
    
    def run(self):
        try:
            self.log_fetch_func()
        except KeyboardInterrupt:
            sys.exit()       

def start_threads(interface, socket):
    CameraThread(interface, socket).start()
    StateThread(interface, socket).start()
    # LogThread(interface, socket).start()
    log.debug("started threads at {}".format(time.time()))
