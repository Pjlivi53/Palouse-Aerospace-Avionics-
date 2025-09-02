import argparse
import os

parser = argparse.ArgumentParser()
parser.add_argument("filename", type=str)
parser.add_argument('-b', '--baud_rate', type=int, required=True)
parser.add_argument('-p', '--serial_port', type=str, required=True)

args = parser.parse_args()

# compile
os.system(f"arduino-cli compile --fqbn arduino:avr:mega {args.filename}")

# upload
os.system(f"arduino-cli upload -p {args.serial_port} --fqbn arduino:avr:mega {args.filename}")

# open serial
os.system(f"screen {args.serial_port} {args.baud_rate}")
