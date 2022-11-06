import sys
import smbus2 as smbus
import time
import sqlite3
import os.path

I2C_SLAVE_ADDRESS1 = 0xf

BASE_DIR = os.path.dirname(os.path.abspath(__file__))
db_path = os.path.join(BASE_DIR, "database.db")

def convertStringsToBytes(src):
    converted = []
    for b in src:
        converted.append(ord(b))
    return converted

def main(args):
    I2Cbus = smbus.SMBus(1)
    conn = sqlite3.connect(db_path)
    print("connected to SQLite")
    with smbus.SMBus(1) as I2Cbus:
        try:
            data1 = I2Cbus.read_i2c_block_data(I2C_SLAVE_ADDRESS1,0x00,20)
            #data comes in as array of bytes, in this format:
            #[flame1,flame2,flame3,flame4,flame5,flame6,flame7,flame8,smoke1,smoke2,smoke3,smoke4,smoke5,smoke6,smoke7,smoke8,motion,humidity,temp1,temp2,water]
            print(str(data1))
            file = open("logfile.txt","w")
            file.write(data1)
            file.close()
            for x in range(1,22):
                conn.execute("UPDATE")
        except:
            print("remove I/O error")
    return 0

if __name__ == "__main__":
    try:
        main(sys.argv)
    except KeyboardInterrupt:
        print("stopped")
    input()

