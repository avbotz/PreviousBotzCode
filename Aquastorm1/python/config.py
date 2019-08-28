import logging

HOST = '0.0.0.0'
WEBSERVER_PORT = 5000
ZMQ_PORT = 5554

FRAMERATE = 5 # How often to fetch images from the sub (in images/second) IN THEORY, in practice it's around 4x less
STATERATE = 1  # How often to fetch state from the sub
LOGRATE = 1  # How often to fetch logs from the sub

# so at 120 theoretical fps we get around 30 actual fps
IDLE_TIME = .5  # How long to continue fetching after all clients disconnect (in seconds)


LOG_FPS = True  # periodically print out fps values
LOGGING_LEVEL = logging.ERROR
FRAMEWORK_LOGGING_LEVEL = logging.ERROR # controls internal server ramblings

F_IDX = 0  # Camera id's
D_IDX = 1
