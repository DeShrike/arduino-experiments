import struct
import base64
import requests

WIDTH = 64
HEIGHT = 32
EXPECTED_SIZE = WIDTH * HEIGHT * 2  # 2 bytes for each uint16_t

# Create a list of values (this is just an example, modify it as needed)
data = []
for i in range(WIDTH * HEIGHT):
    data.append(i)  # You can replace this with actual data

# Ensure the data is in little-endian format by packing as '<H'
# '<' is for little-endian, 'H' is for unsigned 16-bit (uint16_t)
binary_payload = struct.pack('<' + 'H' * len(data), *data)

print(len(binary_payload))

# Verify the payload size is correct (should be 4096 bytes)
assert len(binary_payload) == EXPECTED_SIZE

b64 = base64.b64encode(binary_payload).decode('ascii')

# Send the data as raw binary (octet-stream) in the POST request
url = "http://192.168.0.170/frame"
#response = requests.post(url, data=binary_payload, headers={"Content-Type": "application/octet-stream"})

response = requests.post(
    url,
    data={"data": b64}
)

print("after POST", response.request.headers)

# Print the response from the server
print(response.status_code, response.text)
