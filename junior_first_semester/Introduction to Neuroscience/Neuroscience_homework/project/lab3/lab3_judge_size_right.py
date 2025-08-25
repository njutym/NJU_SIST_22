from psychopy import visual, core, data, gui, event
import random
import pandas as pd
import numpy as np
import os

# 记录被试者
name = {'Your Name': ''}
dlg = gui.DlgFromDict(dictionary=name, title='Circle Comparison Experiment')
if dlg.OK == False:
    core.quit()

# 数据保存路径
filename = f"./data_right/{name['Your Name']}_results.csv"
dataRecords = []

# 创建窗口
win = visual.Window(size=(1200, 1000), color=(1, 1, 1), units="norm")

# 显示开始提示界面
startWarning = visual.TextStim(win, text="按任意键开始实验\n提示:右边圆大的样本占75%", color=(-1, -1, -1), height=0.08)
startWarning.draw()
win.flip()

# 等待按键
event.waitKeys()

# 清除提示界面
win.flip()
core.wait(1.0)  # 显示 1.0 秒
# 设置圆形刺激
circleLeft = visual.Circle(win, radius=0.1, pos=(-0.53, 0), fillColor='blue', lineColor='blue')
circleRight = visual.Circle(win, radius=0.1, pos=(0.53, 0), fillColor='blue', lineColor='blue')

# 创建反应时间文本、“FASTER”提示文本和中央叉号
reactionTimeText = visual.TextStim(win, text="", pos=(0, -0.8), color=(-1, -1, -1), height=0.08)  # 底部显示反应时间
fasterText = visual.TextStim(win, text="FASTER", pos=(-0.15, 0), color='red', height=0.4, bold=True, alignHoriz='center')  # 严格居中的“FASTER”提示
centralX = visual.TextStim(win, text="X", pos=(0, 0), color=(-1, -1, -1), height=0.25)  # 中央显示叉号

# 定义比率 r 的值
ratios = [-0.32, -0.24, -0.16, -0.08, 0, 0.08, 0.16, 0.24, 0.32] * 2 + [0.32, 0.24, 0.16, 0.08] * 4
random.shuffle(ratios)  # 随机打乱顺序

# 主实验循环
for i in range(len(ratios)):
    # 获取圆形大小和位置
    r = ratios[i]
    sizeLeft = random.uniform(0.05, 0.10)
    sizeRight = sizeLeft * (1 + r)
    circleLeft.radius = sizeLeft
    circleRight.radius = sizeRight

    # 重置反应时间和“FASTER”提示
    show_faster = False
    reaction_time_value = None
    correct = 0  # 默认值，稍后更新

    # 显示刺激并记录显示的开始时间
    circleLeft.draw()
    circleRight.draw()
    centralX.draw()
    win.flip()
    
    # 启动计时器并等待响应
    stimulus_onset_time = core.getTime()
    keys = event.waitKeys(maxWait=2, keyList=['f', 'j', 'escape'], timeStamped=True)

    # 检查按键响应
    if keys:
        response, press_time = keys[0]
        reaction_time_value = press_time - stimulus_onset_time  # 精准反应时间
        reactionTimeText.text = f"反应时间: {reaction_time_value:.2f}秒"
        
        # 显示"FASTER"提示的条件
        show_faster = reaction_time_value > 0.6

        # 显示反应时间和"FASTER"提示（如果需要）
        circleLeft.draw()
        circleRight.draw()
        reactionTimeText.draw()
        centralX.draw()
        if show_faster:
            fasterText.draw()
        win.flip()
        core.wait(0.5)  # 保持显示 0.5 秒

        # 处理按键响应
        if response == 'escape':
            win.close()
            core.quit()
        else:
            correct = (response == 'f' and sizeLeft > sizeRight) or (response == 'j' and sizeRight > sizeLeft)
            correct = int(correct)  # 转为0或1
    else:
        # 如果没有响应，则超时处理
        response = 'no_response'
        reaction_time_value = 2.0  # 超时的默认反应时间
        correct = 0
        show_faster = True  # 超时也显示“FASTER”

    # 记录数据
    dataRecords.append({
        'Trial': i + 1,
        'Size_Left': sizeLeft,
        'Size_Right': sizeRight,
        'Response': response,
        'Reaction_Time': reaction_time_value,
        'Correct': correct,
        'Ratio_r': r
    })

    # 显示白屏并显示“FASTER”提示（如果需要）
    win.color = 'white'
    win.flip()
    if show_faster:
        fasterText.draw()
        win.flip()
    core.wait(0.5)  # 显示 0.5 秒

    # 重置窗口背景颜色为白色
    win.color = (1, 1, 1)
    win.flip()

# 保存数据
df = pd.DataFrame(dataRecords)
df.to_csv(filename, index=False)

# 关闭窗口
win.close()
core.quit()