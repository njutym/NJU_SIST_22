x = [[5.9, 3.2], [4.6, 2.9], [6.2, 2.8], [4.7, 3.2], [5.5, 4.2],
     [5.0, 3.0], [4.9, 3.1], [6.7, 3.1], [5.1, 3.8], [6.0, 3.0]]


def dis(center, i):
    temp = center
    a = []
    b = []
    c = []
    for p in x:
        d1 = (p[0]-center[0][0])*(p[0]-center[0][0]) + \
            (p[1]-center[0][1])*(p[1]-center[0][1])
        d2 = (p[0]-center[1][0])*(p[0]-center[1][0]) + \
            (p[1]-center[1][1])*(p[1]-center[1][1])
        d3 = (p[0]-center[2][0])*(p[0]-center[2][0]) + \
            (p[1]-center[2][1])*(p[1]-center[2][1])
        if d1 < d2:
            if d1 < d3:
                a.append(p)
            else:
                c.append(p)
        else:
            if d2 < d3:
                b.append(p)
            else:
                c.append(p)
    center = [[0, 0], [0, 0], [0, 0]]
    for aa in a:
        center[0][0] += aa[0]
        center[0][1] += aa[1]
    center[0][0] /= len(a)
    center[0][1] /= len(a)
    for bb in b:
        center[1][0] += bb[0]
        center[1][1] += bb[1]
    center[1][0] /= len(b)
    center[1][1] /= len(b)
    for cc in c:
        center[2][0] += cc[0]
        center[2][1] += cc[1]
    center[2][0] /= len(c)
    center[2][1] /= len(c)
    print('Turn {}: \n{}\n{}\n{}\n{}'.format(i, a, b, c, centerzc))
    if not temp == center:
        dis(center, i+1)


dis([[6.2, 3.2], [6.6, 3.7], [6.5, 3.0]], 0)
