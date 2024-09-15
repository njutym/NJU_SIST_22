import reflex
from time import sleep
import random


def dash_left():
    reflex.act(0b10000001)
    sleep(0.01)
    reflex.act(0)


def dash_right():
    reflex.act(0b10000010)
    sleep(0.01)
    reflex.act(0)


def walk_left():
    reflex.act(1)
    sleep(random.uniform(0, 2))


def walk_right():
    reflex.act(2)
    sleep(random.uniform(0, 2))


def jump(direct=0):
    reflex.act(0b100+direct)
    sleep(0.01)
    reflex.act(0)


def jump_high(direct=0):
    reflex.act(0b10000100+direct)
    sleep(0.01)
    reflex.act(0)


def fly():
    reflex.act(0b10000100)
    sleep(0.2)
    reflex.act(0b10000000)


def squat():
    reflex.act(0b1000)


def stop():
    reflex.act(0)


def hit():
    reflex.act(0b10000)
    sleep(0.01)
    reflex.act(0)


def hit_down():
    reflex.act(0b11000)
    sleep(0.01)
    reflex.act(0)


def hit_left():
    reflex.act(0b10001)
    sleep(0.01)
    reflex.act(0)


def hit_right():
    reflex.act(0b10010)
    sleep(0.01)
    reflex.act(0)


def shoot(hold=False):
    reflex.act(0b100000)
    sleep(0.15 if hold else 0.01)
    reflex.act(0)


def shoot_down():
    reflex.act(0b101000)
    sleep(0.01)
    reflex.act(0)


def shoot2(hold=False):
    reflex.act(0b1000000)
    sleep(0.3 if hold else 0.01)
    reflex.act(0)


def shoot2_down():
    reflex.act(0b1001000)
    sleep(0.01)
    reflex.act(0)


def kick_air():
    reflex.act(0b10100)
    sleep(0.01)
    reflex.act(0)


def kick_down(hold=False):
    reflex.act(0b11010)
    sleep(0.2 if hold else 0.01)
    reflex.act(0)


def ex_1(hold=False):
    reflex.act(0b1000)
    sleep(0.01)
    reflex.act(0b1010)
    sleep(0.01)
    reflex.act(0b10)
    sleep(0.01)
    reflex.act(0b100000)
    sleep(0.7 if hold else 0.01)
    reflex.act(0)


def ex_2(hold=False):
    reflex.act(0b1000)
    sleep(0.01)
    reflex.act(0b1001)
    sleep(0.01)
    reflex.act(0b1)
    sleep(0.01)
    reflex.act(0b100000)
    sleep(0.7 if hold else 0.01)
    reflex.act(0)


def ex_3(hold=False):
    reflex.act(0b10)
    sleep(0.01)
    reflex.act(0b1000)
    sleep(0.01)
    reflex.act(0b1010)
    sleep(0.01)
    reflex.act(0b100000)
    sleep(0.7 if hold else 0.01)
    reflex.act(0)


def ex_4(hold=False):
    reflex.act(0b1)
    sleep(0.01)
    reflex.act(0b1000)
    sleep(0.01)
    reflex.act(0b1001)
    sleep(0.01)
    reflex.act(0b100000)
    sleep(0.7 if hold else 0.01)
    reflex.act(0)


def apply(action):
    if action == 0:
        walk_left()
    elif action == 1:
        walk_right()
    elif action == 2:
        dash_left()
    elif action == 3:
        dash_right()
    elif action == 4:
        jump(0)
    elif action == 5:
        jump(1)
    elif action == 6:
        jump(2)
    elif action == 7:
        jump_high(0)
    elif action == 8:
        jump_high(1)
    elif action == 9:
        jump_high(2)
    elif action == 10:
        squat()
    elif action == 11:
        hit()
    elif action == 12:
        hit_left()
    elif action == 13:
        hit_right()
    elif action == 14:
        hit_down()
    elif action == 15:
        shoot()
    elif action == 16:
        shoot(1)
    elif action == 17:
        shoot2()
    elif action == 18:
        shoot2(1)
    elif action == 19:
        shoot_down()
    elif action == 20:
        shoot2_down()
    elif action == 21:
        kick_air()
    elif action == 22:
        kick_down()
    elif action == 23:
        kick_down(1)
    elif action == 24:
        ex_1(0)
    elif action == 25:
        ex_1(1)
    elif action == 26:
        ex_2()
    elif action == 27:
        ex_2(1)
    elif action == 28:
        ex_3()
    elif action == 29:
        ex_4()
    elif action == 30:
        fly()
    else:
        stop()
