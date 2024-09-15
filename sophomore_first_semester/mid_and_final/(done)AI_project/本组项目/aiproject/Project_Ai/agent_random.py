import reflex
import time
import random
import Info


def randset():
    for k in reflex.keys.keys():
        reflex.keys[k] = random.random() > 0.5


def agent():
    Info.init()
    print('3')
    time.sleep(1)
    print('2')
    time.sleep(1)
    print('1')
    time.sleep(1)
    print('READY')
    while True:
        randset()
        reflex.apply()
        time.sleep(0.1)
        Info.show()
        print(reflex.keys)
    reset()
    apply()


agent()
