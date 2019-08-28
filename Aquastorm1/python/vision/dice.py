import numpy as np
import os
import six.moves.urllib as urllib
import sys
import tarfile
import tensorflow as tf
import zipfile
import pickle
import time 
import zmq  

from collections import defaultdict
from io import StringIO
from matplotlib import pyplot as plt
from PIL import Image

if tf.__version__ < '1.4.0':
    raise ImportError('Please upgrade your tensorflow installation to v1.4.0!')

sys.path.append("..")

from utils import label_map_util

from utils import visualization_utils as vis_util

MODEL_NAME = 'dice_inference_graph'

PATH_TO_CKPT = MODEL_NAME + 'python/object_detection/dice_inference_graph/frozen_inference_graph.pb'

PATH_TO_LABELS = os.path.join('data', 'object-detection.pbtxt')

NUM_CLASSES = 4

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

detection_graph = tf.Graph()

with detection_graph.as_default():
    od_graph_def = tf.GraphDef()
    with tf.gfile.GFile(PATH_TO_CKPT, 'rb') as fid:
        serialized_graph = fid.read()
        od_graph_def.ParseFromString(serialized_graph)
        tf.import_graph_def(od_graph_def, name='')

label_map = label_map_util.load_labelmap(PATH_TO_LABELS)
categories = label_map_util.convert_label_map_to_categories(label_map, max_num_classes=NUM_CLASSES, use_display_name=True)
category_index = label_map_util.create_category_index(categories)

def load_image_into_numpy_array(image):
    (im_width, im_height) = image.size
    return np.array(image).reshape(
        (im_height, im_width, 3)).astype(np.uint8)

with detection_graph.as_default():
    #Experimental XLA implementation, if errors comment out next two lines and delete config=config
    config = tf.ConfigProto()
    config.graph_options.optimizer_options.global_jit_level = tf.OptimizerOptions.ON_1
    with tf.Session(graph=detection_graph, config=config) as sess:
        # Definite input and output Tensors for detection_graph
        image_tensor = detection_graph.get_tensor_by_name('image_tensor:0')
        # Each box represents a part of the image where a particular object was detected.
        detection_boxes = detection_graph.get_tensor_by_name('detection_boxes:0')
        # Each score represent how level of confidence for each of the objects.
        # Score is shown on the result image, together with the class label.
        detection_scores = detection_graph.get_tensor_by_name('detection_scores:0')
        detection_classes = detection_graph.get_tensor_by_name('detection_classes:0')
        num_detections = detection_graph.get_tensor_by_name('num_detections:0')

    while (True):
        message = socket.recv()
        type64 = base64.b64decode(message)
        npimg = np.fromstring(type64, dtype=np.uint8)
        image = cv2.imdecode(npimg, 1)
        image_np = load_image_into_numpy_array(image)
        image_np_expanded = np.expand_dims(image_np, axis=0)
        (boxes, scores, classes, num) = sess.run(
            [detection_boxes, detection_scores, detection_classes, num_detections],
            feed_dict={image_tensor: image_np_expanded})
        width, height = image.size
        newbox = np.squeeze(boxes)
        finalboxes = newbox[~(newbox==0).all(1)]
        print(finalboxes)
        numboxes = len(finalboxes)
        for x in range(0,numboxes):
            ymin = boxes[0][x][0]*height
            xmin = boxes[0][x][1]*width
            ymax = boxes[0][x][2]*height
            xmax = boxes[0][x][3]*width
            print("Box " + str(x))
            print('Top left')
            print (xmin,ymin)
            print('Bottom right')
            print (xmax,ymax)
            print(" ")
