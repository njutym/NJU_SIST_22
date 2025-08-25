from psychopy import visual, core, event, data, gui
import random
import pandas as pd
import os

# 设置实验信息
exp_info = {'Participant': ''}
dlg = gui.DlgFromDict(dictionary=exp_info, title='Circle Comparison Experiment')
if dlg.OK == False:
    core.quit()

# 设置数据记录
filename = f"./data/{exp_info['Participant']}_results.csv"
data_records = []

# 创建窗口
win = visual.Window(size=(1000, 800), color=(1, 1, 1), units="norm")

# 显示开始提示界面
start_text = visual.TextStim(win, text="按任意键开始实验", color=(-1, -1, -1))
start_text.draw()
win.flip()

# 等待按键开始实验
event.waitKeys()

# 设置圆形刺激
circle_left = visual.Circle(win, radius=0.1, pos=(-0.62, 0), fillColor='blue', lineColor='blue')
circle_right = visual.Circle(win, radius=0.1, pos=(0.62, 0), fillColor='blue', lineColor='blue')
center_cross = visual.TextStim(win, text='×', pos=(0, 0), color=(-1, -1, -1), height=0.1)

# 定义比率 r 的值
ratios = [-0.32, -0.24, -0.16, -0.08, 0, 0.08, 0.16, 0.24, 0.32] * 4
random.shuffle(ratios)  # 随机打乱顺序

# 主实验循环
for trial in range(len(ratios)):
    # 根据 r 计算左右圆的大小
    r = ratios[trial]
    size_left = random.uniform(0.05, 0.2)  # 随机生成左圆大小
    size_right = size_left * (1 + r)  # 计算右圆大小
    circle_left.radius = size_left
    circle_right.radius = size_right
    
    # 绘制刺激
    center_cross.draw()  # 先绘制中心的“×”
    circle_left.draw()
    circle_right.draw()
    win.flip()
    
    # 计时器重置
    reaction_clock = core.Clock()
    
    # 获取响应
    keys = event.waitKeys(maxWait=2, keyList=['left', 'right', 'escape'], timeStamped=reaction_clock)
    
    # 判断按键与大小关系
    response = None
    correct = None
    reaction_time = None
    if keys:
        response, reaction_time = keys[0]  # 键和反应时间
        if response == 'escape':
            break  # 提前退出实验
        correct = (response == 'left' and size_left > size_right) or (response == 'right' and size_right > size_left)
        correct = int(correct)  # 转为0或1
    else:
        response = 'no_response'
        correct = 0  # 超时记为错误
        reaction_time = 2  # 记录超时反应时间为2
    
    # 记录数据
    data_records.append({
        'Trial': trial + 1,
        'Size_Left': size_left,
        'Size_Right': size_right,
        'Response': response,
        'Reaction_Time': reaction_time,
        'Correct': correct,
        'Ratio_r': r  # 记录比率 r
    })
    
    # 短暂延迟
    core.wait(0.5)

# 保存数据
df = pd.DataFrame(data_records)
df.to_csv(filename, index=False)

# 关闭窗口
win.close()
core.quit()