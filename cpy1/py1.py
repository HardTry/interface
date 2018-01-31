# for linux/unix, copy this file to /usr/lib/python2.7/site-packages
# for windows, copy this file to current runtime folder


def multiply(a,b):
    print ("Will compute", a, "times", b)
    c = 0
    for i in range(0, a):
        c = c + b
    return c
