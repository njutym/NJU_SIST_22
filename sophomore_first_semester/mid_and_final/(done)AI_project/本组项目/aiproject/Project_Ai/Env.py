import Info
import pynput
import time
import control
kb = pynput.keyboard.Controller()

def tap(key):
    kb.press(key)
    time.sleep(0.1)
    kb.release(key)
    time.sleep(0.1)

class Env():
    observation_space = 14
    action_space = 31

    def reset(self):
        tap(pynput.keyboard.Key.esc)
        tap(pynput.keyboard.Key.down)
        tap(pynput.keyboard.Key.down)
        tap('z')
        time.sleep(2)
        tap('z')
        time.sleep(0.5)
        tap('z')
        time.sleep(5)
        Info.renew()
        return Info.obv()

    def step(self, action):
        control.apply(action)
        time.sleep(0.016)
        Info.renew()
        return (Info.obv(), Info.eval(), Info.done(), 0)
