a = [1,2,3,4,5,6]
b = [1,2,3,4,5,6]
c = [1,2,3,4,5,6]

cnt = list()

for i in a:
    for j in b:
        for k in c:
            cnt.append(i+j+k)

for item in cnt:
    print(item,end = ' ')

ans = dict()

for i in range(3,19):
    ans.setdefault(i,cnt.count(i))

print(ans)

print(1+3+6+10+15+21+25+27+27+25+21+15+10+6+3+1)

print("小:",(3+6+10+15+21+25+27-2)/216)

print("大:",(27+25+21+15+10+6+3-2)/216)

for i in range(4,18):
    print(i,":",cnt.count(i)/216)

print("单张:",1-((5/6)**3))

print("两张不一样:",2*3*1/6*1/6*1/6+6*1/6*1/6*4/6)

print("对子:",(1/6)**3+3*1/6/6*5/6)

print("豹子:",(1/6)**3)

print("全围:",1/6/6)

print("三面猜对，除去全围:",((1/6)**3+3*1/6/6*5/6)*6*1/6/6/6*3+(1-1/6/6-(1/6)**3+3*1/6/6*5/6)*1/6/6/6)