import json
import sys


def localmain(src,dest):
    with open(src , "r") as json_file:
        data = json.load(json_file)
    json_read(data, dest)

def json_read(data, dest):
    fout = open(dest,"w")
    fout.write(json.dumps(data))
    fout.close()

if len(sys.argv) != 3:
    print("usage: python input.json_file output_file_location")
else:
    localmain(sys.argv[1],sys.argv[2])

