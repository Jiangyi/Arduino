import struct
import sys

import serial
import csv
import time

labels = ["X", "Y", "Z"]
accumulator = [0] * 3


def sendBytesToUART(arduino, int_val):
    bytes = struct.pack("<l", int_val)
    # print(bytes)
    arduino.write(bytes)


def WaitForUARTReply(arduino):
    read_byte = arduino.read()
    while int.from_bytes(read_byte, "little") != 42:
        # print(line)
        time.sleep(0.1)
        read_byte = arduino.readline()


def sendToUART(arduino, row):
    for i in range(3):
        val = int(row[i])
        print("Sending {}: {}".format(labels[i], val))
        sendBytesToUART(arduino, val)
        accumulator[i] += val


def returnToZero(arduino):
    print("Returning to 0 values")
    for i in range(3):
        print("Sending {}: {}".format(labels[i], -accumulator[i]))
        sendBytesToUART(arduino, -accumulator[i])


if __name__ == '__main__':
    if len(sys.argv) != 2:
        exit(-1)

    filename = sys.argv[1]
    arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1)

    # For some reason, this is needed for the Arduino UART connection to actually be ready to accept transmission
    time.sleep(2)

    with open(filename, newline='') as csvfile:
        instruction_reader = csv.reader(csvfile, dialect='excel')
        next(instruction_reader, None)  # skip headers
        for row in instruction_reader:
            sendToUART(arduino, row)

    returnToZero(arduino)
    arduino.close()  # close port
