import argparse

import sys 
sys.path.append("..") 
from useless_python_lib.throughput_monitor import *

argparser = argparse.ArgumentParser()
argparser.add_argument('filename', metavar='filename', type=str, help='feature filename')
args = argparser.parse_args()

draw_bandwidth_fig(args.filename, "result/bandwidth_bps.pdf", "result/bandwidth_pps.pdf")