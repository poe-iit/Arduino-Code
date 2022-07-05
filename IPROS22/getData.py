import sys
import smbus2 as smbus
import time

I2C_SLAVE_ADDRESS1 = 0xf
I2C_SLAVE_ADDRESS2 = 0x10
I2C_SLAVE_ADDRESS3 = 0x11


def convertStringsToBytes(src):
    converted = []
    for b in src:
        converted.append(ord(b))
    return converted

def main(args):
    I2Cbus = smbus.SMBus(1)
    with smbus.SMBus(1) as I2Cbus:
        try:
            data1 = I2Cbus.read_i2c_block_data(I2C_SLAVE_ADDRESS1,0x00,14)
            print(str(data1))
            data2 = I2Cbus.read_i2c_block_data(I2C_SLAVE_ADDRESS2,0x00,16)
            print(str(data2))
            data3 = I2Cbus.read_i2c_block_data(I2C_SLAVE_ADDRESS3,0x00,14)
            print(str(data3))
            file = open("logfile.txt","w")
            file.write(data1)
            file.write(data2)
            file.write(data3)
            file.close()
        except:
            print("remove I/O error")
    return 0

if __name__ == "__main__":
    try:
        main(sys.argv)
    except KeyboardInterrupt:
        print("stopped")
    input()

