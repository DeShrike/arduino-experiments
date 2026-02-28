import struct
import base64
import requests
import math
import random
import time

FRAMEURL = "http://192.168.0.170/frame"
TEXTURL = "http://192.168.0.170/text"

WIDTH = 64
HEIGHT = 32
data = [ 0 for _ in range(WIDTH * HEIGHT) ]

primes = [3,5,7,11,13,17,23,29]

EXPECTED_SIZE = WIDTH * HEIGHT * 2  # 2 bytes for each uint16_t

def hsv_to_rgb(h, s, v):
    h = float(h)
    s = float(s)
    v = float(v)

    c = v * s
    x = c * (1 - abs((h / 60.0) % 2 - 1))
    m = v - c

    if h < 60:
        r, g, b = c, x, 0
    elif h < 120:
        r, g, b = x, c, 0
    elif h < 180:
        r, g, b = 0, c, x
    elif h < 240:
        r, g, b = 0, x, c
    elif h < 300:
        r, g, b = x, 0, c
    else:
        r, g, b = c, 0, x

    return (
        int((r + m) * 255),
        int((g + m) * 255),
        int((b + m) * 255),
    )

def sendFrame():
	# Ensure the data is in little-endian format by packing as '<H'
	# '<' is for little-endian, 'H' is for unsigned 16-bit (uint16_t)
	binary_payload = struct.pack('<' + 'H' * len(data), *data)
	assert len(binary_payload) == EXPECTED_SIZE
	b64 = base64.b64encode(binary_payload).decode('ascii')
	response = requests.post(FRAMEURL, data={"data": b64})
	print(response.status_code, response.text)


def sendTest():
	response = requests.post(TEXTURL, data={"text": "<>?*-+/%!()[]{}@&_\"(')#|"})
	print(response.status_code, response.text)

def IX(x: int, y: int) -> int:
	return x + (y * WIDTH)

def Color(r: int, g: int, b: int) -> int:
    r = max(0, min(255, r))
    g = max(0, min(255, g))
    b = max(0, min(255, b))

    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def fill_rainbow(offset=0):
    for y in range(HEIGHT):
        for x in range(WIDTH):
            hue = (x * 360 // WIDTH + offset) % 360
            r, g, b = hsv_to_rgb(hue, 1.0, 1.0)
            data[IX(x, y)] = Color(r, g, b)

def fill_plasma(t):
    for y in range(HEIGHT):
        for x in range(WIDTH):
            v = (
                math.sin(x * 0.2 + t) +
                math.sin(y * 0.2 + t) +
                math.sin((x + y) * 0.2 + t)
            )
            hue = int((v + 3) * 60) % 360
            r, g, b = hsv_to_rgb(hue, 1, 1)
            data[IX(x, y)] = Color(r, g, b)

def fill_radial_pulse(t):
    cx = WIDTH / 2
    cy = HEIGHT / 2

    for y in range(HEIGHT):
        for x in range(WIDTH):
            dx = x - cx
            dy = y - cy
            dist = math.sqrt(dx*dx + dy*dy)

            v = math.sin(dist * 0.5 - t)
            brightness = (v + 1) / 2

            r = int(255 * brightness)
            b = int(255 * (1 - brightness))
            data[IX(x, y)] = Color(r, 0, b)

def fill_fire(t):
    for y in range(HEIGHT):
        for x in range(WIDTH):
            noise = math.sin(x * 0.3 + t) + math.sin(y * 0.5 + t * 1.5)
            intensity = (noise + 2) / 4

            r = int(255 * intensity)
            g = int(150 * intensity)
            b = int(50 * intensity * 0.3)

            data[IX(x, HEIGHT - y - 1)] = Color(r, g, b)

def fill_stars():
    for i in range(WIDTH * HEIGHT):
        data[i] = Color(0, 0, 10)  # dark background

    for _ in range(40):
        x = random.randrange(WIDTH)
        y = random.randrange(HEIGHT)
        brightness = random.randint(150, 255)
        data[IX(x, y)] = Color(brightness, brightness, brightness)

t = 0
for _ in range(1000):
    #fill_plasma(t)
    #fill_radial_pulse(t)
    #fill_fire(t)
    fill_stars()
    sendFrame()
    t += 0.1
