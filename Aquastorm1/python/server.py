from flask import Flask, render_template
from flask_socketio import SocketIO

import eventlet
import logging

import config
import stream
import interface

eventlet.sleep(0)
eventlet.monkey_patch()

logging.basicConfig(level=config.LOGGING_LEVEL)
log = logging.getLogger(__name__)
for i in ['werkzeug', 'engineio', 'socketio']:
    logging.getLogger(i).setLevel(config.FRAMEWORK_LOGGING_LEVEL)

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret af'
socket = SocketIO(app)

@app.before_first_request
def setup():
    stream.start_threads(interface.Interface(), socket)


@app.route("/")
def index():
    return render_template('index.html')


if __name__ == "__main__":
    log.info("Webserver is running, waiting for first request on {0}:{1}".format(config.HOST, config.WEBSERVER_PORT))
    socket.run(app, host=config.HOST, port=config.WEBSERVER_PORT, debug=False)
