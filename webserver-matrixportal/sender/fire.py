import struct
import base64
import requests
import math
import random
import time

FRAMEURL = "http://192.168.0.170/frame"

WIDTH = 64
HEIGHT = 32

BRIGHTNESS = 0.30   # 0.0 = off, 1.0 = full power

data = [ 0 for _ in range(WIDTH * HEIGHT) ]

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
	binary_payload = struct.pack('<' + 'H' * len(data), *data)
	assert len(binary_payload) == EXPECTED_SIZE
	b64 = base64.b64encode(binary_payload).decode('ascii')
	response = requests.post(FRAMEURL, data={"data": b64})
	print(response.status_code, response.text)

def IX(x: int, y: int) -> int:
	return x + (y * WIDTH)

def Color(r: int, g: int, b: int) -> int:
    r = max(0, min(255, r))
    g = max(0, min(255, g))
    b = max(0, min(255, b))

    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)



# Heat buffer (0-255 per pixel)
fire_heat = [0 for _ in range(WIDTH * HEIGHT)]

FIRE_HEIGHT = 25

def update_fire():
    # Update base fire heat (only for the bottom row)
    for x in range(WIDTH):
        fire_heat[IX(x, HEIGHT - 1)] = random.randint(200, 255)  # Flame base

    # Propagate fire heat upward (only for the `FIRE_HEIGHT` rows)
    for y in range(HEIGHT - 1 - FIRE_HEIGHT, HEIGHT - 1):
        for x in range(WIDTH):
            src = IX(x, y + 1)
            decay = random.randint(0, 40)
            dst_x = x + random.randint(-1, 1)

            if dst_x < 0:
                dst_x = 0
            if dst_x >= WIDTH:
                dst_x = WIDTH - 1

            dst = IX(dst_x, y)

            new_heat = fire_heat[src] - decay
            if new_heat < 10:
                new_heat = 0
            fire_heat[dst] = new_heat

def heat_to_color(value):
    if value < 85:
        r = value * 3
        g = 0
        b = 0
    elif value < 170:
        r = 255
        g = (value - 85) * 3
        b = 0
    else:
        r = 255
        g = 255
        b = (value - 170) * 3

    # Apply brightness scaling (optional)
    r = int(r * BRIGHTNESS)
    g = int(g * BRIGHTNESS)
    b = int(b * BRIGHTNESS)

    return Color(r, g, b)

def render_fire():
    for y in range(HEIGHT):
        for x in range(WIDTH):
            data[IX(x, y)] = heat_to_color(fire_heat[IX(x, y)])


for _ in range(1000):
    update_fire()
    render_fire()
    sendFrame()
