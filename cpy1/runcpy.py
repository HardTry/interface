import py1
from ctypes import *

cpy1 = cdll.LoadLibrary("libcpy1.so")
print(cpy1.run_multiply)

p1 = c_int(2)
p2 = c_int(3)
mp = py_object(py1.multiply)

print(py1, py1.multiply, mp, p1, p2)

#cpy1.run_multiply(mp, p1, p2)
cpy1.run_multiply()
