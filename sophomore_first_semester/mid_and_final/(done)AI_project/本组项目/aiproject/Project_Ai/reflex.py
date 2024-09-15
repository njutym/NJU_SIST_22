import pynput
kb = pynput.keyboard.Controller()
keys = {
    pynput.keyboard.Key.left: False,
    pynput.keyboard.Key.right: False,
    pynput.keyboard.Key.up: False,
    pynput.keyboard.Key.down: False,
    'z': False,
    'x': False,
    'c': False,
    'a': False,
    's': False,
    'd': False
}

def reset():
    for k in keys.keys():
        keys[k] = False


def trans(action):
    for k in keys.keys():
        keys[k] = (action & 1) != 0
        action >>= 1


def apply():
    for k, v in keys.items():
        if v:
            kb.press(k)
        else:
            kb.release(k)
            pass


def act(action):
    trans(action)
    apply()
