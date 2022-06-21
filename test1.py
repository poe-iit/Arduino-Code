import sys
import smbus2 as smbus
import time

I2C_SLAVE_ADDRESS1 = 0xf


def convertStringsToBytes(src):
    converted = []
    for b in src:
        converted.append(ord(b))
    return converted

def main(args):
    I2Cbus = smbus.SMBus(1)
    with smbus.SMBus(1) as I2Cbus:
        slaveAddress = I2C_SLAVE_ADDRESS1
        cmd = input("Enter command: ")
        #BytesToSend = convertStringsToBytes(cmd)
        #print("Sent " + str(slaveAddress) + "the " +"'"+str(cmd)+"'"+" command.")
        #I2Cbus.write_i2c_block_data(slaveAddress,0x00,BytesToSend)
        #time.sleep(.75)
        if cmd == "m" or "measure":
            while True: 
                try:
                    data = I2Cbus.read_i2c_block_data(slaveAddress,0x00,4)
                    print("received from slave: " + str(data))
                    time.sleep(1)
                except:
                    print("remote i/o error")
                    time.sleep(0.5)
    return 0

if __name__ == "__main__":
    try:
        main(sys.argv)
    except KeyboardInterrupt:
        print("stopped")
    input()

