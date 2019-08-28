import sys
import os
import math

import bpy
import bgl
import gpu

# add this file's directory to path because blender does not add it by default
sys.path.append(os.path.dirname(os.path.abspath(__file__))[:-16] + "/python")
# also add the blender's file's directory to path
sys.path.append(os.path.dirname(bpy.data.filepath))

import numpy as np
import cv2
import interface
import config


def generateNodes(scene):
    
    # enable nodes
    scene.use_nodes = True
    tree = scene.node_tree
    links = tree.links

    # clear nodes
    for n in tree.nodes:
        tree.nodes.remove(n)

    # input node
    render = tree.nodes.new('CompositorNodeRLayers')
    render.location = 185, 285

    # output node
    viewer = tree.nodes.new('CompositorNodeViewer')
    viewer.location = 750, 210
    viewer.use_alpha = False

    # output compositing node (for rendering from blender ui)
    composite = tree.nodes.new('CompositorNodeComposite')
    composite.location = 750, 0
    composite.use_alpha = False

    # connect nodes
    links.new(render.outputs[0], viewer.inputs[0])
    links.new(render.outputs[0], composite.inputs[0])


def renderFrame(scene, camera):
    render = scene.render

    # set camera
    scene.camera = camera

    # render frame
    bpy.ops.render.render()

    # fetch image data
    node = bpy.data.images['Viewer Node']
    buffer = np.array(node.pixels[:])

    # return numpy array
    return (255 * buffer).astype(np.uint8).reshape([
        int(render.resolution_y * render.resolution_percentage/100), int(render.resolution_x * render.resolution_percentage/100), 4
    ])[:,:,[2,1,0]].transpose([0,1,2])[::-1,:,:]


def run():

    print("Running simulator!")
    connect = interface.Interface()
    for m in bpy.data.materials:
        m.use_shadeless = False
        m.use_shadows = False
        m.use_cast_shadows = False
        m.use_cast_buffer_shadows = False
        m.use_cast_approximate = False
        m.use_raytrace = False
        m.subsurface_scattering.use = False

    scene = bpy.data.scenes[0]
    sub = scene.objects['Submarine']
    loc = sub.location
    att = sub.rotation_euler

    cam_f = scene.objects['CameraFront']
    cam_d = scene.objects['CameraDown']

    generateNodes(scene)

    quit = False
    while True:
        if (quit):
            bpy.context.area.type = original_type
            input.close()
            print("Exiting simulator...")
            return 0

        # pretend control is constant, move us to exact desired location 
        state = connect.get_desired_state()
        print(state)

        # movement shouldn't be instant, roughly ~15%
        loc[0] = loc[0] + 0.15 * (state[0] - loc[0])
        loc[1] = loc[1] + 0.15 * (state[1] - loc[1])
        loc[2] = loc[2] + 0.15 * (state[2] - loc[2])
        att[1] = state[3] * 2*math.pi
        att[0] = state[4] * 2*math.pi
        att[2] = state[5] * 2*math.pi

        # set that desired location back in aquastorm
        print (type([]))
        connect.set_current_state([loc[0], loc[1], loc[2], att[1], att[0], att[2]])
        
        img_f = renderFrame(scene, cam_f)
        img_d = renderFrame(scene, cam_d)

        connect.set_image(config.F_IDX, img_f)
        connect.set_image(config.D_IDX, img_d)

run()

