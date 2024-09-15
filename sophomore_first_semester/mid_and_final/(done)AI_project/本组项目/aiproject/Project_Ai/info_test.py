import pymem
exe=pymem.Pymem("th123_beta.exe")
hp_base=[0,0]
mp_base=[0,0]
xp_base=[0,0]
behavior_base=[0,0]
hitbox_base=[[0,0],[0,0]]
def init():
    exe=pymem.Pymem("th123_beta.exe")
    hp_base[0]=exe.read_int(0x8855c8)+1188
    hp_base[1]=exe.read_int(0x8855c8)+1436
    mp_base[0]=exe.read_int(0x88562c)+940
    mp_base[1]=exe.read_int(0x885630)+940
    xp_base[0]=mp_base[0]+328
    xp_base[1]=mp_base[1]+328
    hitbox_base[0][0]=mp_base[0]-396
    hitbox_base[1][0]=mp_base[1]-396
    hitbox_base[0][1]=mp_base[0]-712
    hitbox_base[1][1]=mp_base[1]-712
    behavior_base[0]=mp_base[0]-724
    behavior_base[1]=mp_base[1]-724
def gethp(i):
    return exe.read_int(hp_base[i])
def getmp(i):
    return exe.read_int(mp_base[i])
def getmp(i):
    return exe.read_int(mp_base[i])
def getlocation(i):
    return [exe.read_int(hitbox_base[i][0]),exe.read_int(hitbox_base[i][1])]
def getxp(i):
    return exe.read_int(xp_base[i])-83886080
def getbehavior(i):
    return exe.read_short(behavior_base[i])
def live():
    init()
    hp_record_0=gethp(0)
    hp_record_1=gethp(1)
    mp_record_0=getmp(0)
    mp_record_1=getmp(1)
    while hp_record_0 and hp_record_1:
        hp_0=gethp(0)
        hp_1=gethp(1)
        mp_0=getmp(0)
        mp_1=getmp(1)
        if not hp_0==hp_record_0:
            print("Player_1 hurt: {}, now: {}".format(hp_0-hp_record_0,hp_0))
            hp_record_0=hp_0
        if not hp_1==hp_record_1:
            print("Player_2 hurt: {}, now: {}".format(hp_1-hp_record_1,hp_1))
            hp_record_1=hp_1
        if mp_0<mp_record_0:
            print("Player_1 spelling: {}, now: {}".format(mp_0-mp_record_0,mp_0))
            mp_record_0=mp_0
        if mp_1<mp_record_1:
            print("Player_2 spelling: {}, now: {}".format(mp_1-mp_record_1,mp_1))
            mp_record_1=mp_1
    print("Game over")
live()