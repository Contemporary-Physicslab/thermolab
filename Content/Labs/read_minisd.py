import serial
import time

PORT = "COM16"
BAUD = 115200

with serial.Serial(PORT, BAUD, timeout=10) as ser:

    # Allow the Arduino to reset after connecting
    time.sleep(2)

    # Clear any startup messages
    ser.reset_input_buffer()

    # Request the CSV
    ser.write(b'D')

    print("Waiting for CSV data...")

    while True:
        line = ser.readline()

        if not line:
            raise TimeoutError("Arduino did not respond")

        if line.strip() == b"BEGIN_DATA":
            break

    with open("DATA.CSV", "wb") as f:
        while True:
            line = ser.readline()

            if not line:
                raise TimeoutError("CSV transfer timed out")

            if line.strip() == b"END_DATA":
                break

            f.write(line)

print("DATA.CSV saved successfully!")