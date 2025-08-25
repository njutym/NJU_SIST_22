from psychopy import visual, core, data, gui, event
import random
import pandas as pd
import os

# 记录被试者
name = {'Your Name': ''}
dlg = gui.DlgFromDict(dictionary=name, title='Circle Comparison Experiment')
if dlg.OK == False:
    core.quit()

# 参数设置
filename = f"./data/{name['Your Name']}_results.csv"
display_interval = 5  # 每多少个椭圆显示一个数字
question_count = 2  # 需要用户判断多少个数字
max_attempts = 2  # 错误输入的最大重试次数

dataRecords = []

# 创建窗口
win = visual.Window(size=(1200, 1000), color=(1, 1, 1), units="norm")

# 显示开始提示界面
startWarning = visual.TextStim(win, text="按任意键开始实验", color=(-1, -1, -1))
startWarning.draw()
win.flip()

# 等待按键
event.waitKeys()

# 清除提示界面
win.flip()
core.wait(1.0)  # 显示 1.0 秒

# 设置圆形刺激
circleLeft = visual.Circle(win, radius=0.1, pos=(-0.62, 0), fillColor='blue', lineColor='blue')
circleRight = visual.Circle(win, radius=0.1, pos=(0.62, 0), fillColor='blue', lineColor='blue')

# 创建反应时间文本、“FASTER”提示文本和中央叉号
reactionTimeText = visual.TextStim(win, text="", pos=(0, -0.8), color=(-1, -1, -1), height=0.08)
fasterText = visual.TextStim(win, text="FASTER", pos=(-0.15, 0), color='red', height=0.4, bold=True, alignHoriz='center')
centralX = visual.TextStim(win, text="X", pos=(0, 0), color=(-1, -1, -1), height=0.25)

# 定义比率 r 的值
ratios = [-0.32, -0.24, -0.16, -0.08, 0, 0.08, 0.16, 0.24, 0.32] * 4
random.shuffle(ratios)

# 数字刺激列表
random_numbers = []
number_display_indices = []

# 显示第一个随机数字
initial_number = random.randint(0, 9)
random_numbers.append(initial_number)
number_display_indices.append(1)  # 初始数字的索引设为1
initialText = visual.TextStim(win, text=str(initial_number), color=(-1, -1, -1), height=0.3)
initialText.draw()
win.flip()
core.wait(1.0)

# 主实验循环
for i in range(len(ratios)):
    # 获取圆形大小和位置
    r = ratios[i]
    sizeLeft = random.uniform(0.10, 0.20)
    sizeRight = sizeLeft * (1 + r)
    circleLeft.radius = sizeLeft
    circleRight.radius = sizeRight

    # 重置反应时间和“FASTER”提示
    show_faster = False
    reaction_time_value = None
    correct = 0

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
        reaction_time_value = press_time - stimulus_onset_time
        reactionTimeText.text = f"反应时间: {reaction_time_value:.2f}秒"
        
        # 显示"FASTER"提示的条件
        show_faster = reaction_time_value > 0.6

        # 显示反应时间和"FASTER"提示
        circleLeft.draw()
        circleRight.draw()
        reactionTimeText.draw()
        centralX.draw()
        if show_faster:
            fasterText.draw()
        win.flip()
        core.wait(0.5)

        if response == 'escape':
            win.close()
            core.quit()
        else:
            correct = (response == 'f' and sizeLeft > sizeRight) or (response == 'j' and sizeRight > sizeLeft)
            correct = int(correct)
    else:
        response = 'no_response'
        reaction_time_value = 2.0
        correct = 0
        show_faster = True

    # 每隔 display_interval 个椭圆后生成一个新数字
    if (i + 1) % display_interval == 0:
        random_number = random.randint(0, 9)
        while random_number in random_numbers:  # 确保数字不重复
            random_number = random.randint(0, 9)
        random_numbers.append(random_number)
        number_display_indices.append(len(number_display_indices) + 1)  # 更新数字的索引
        numberText = visual.TextStim(win, text=str(random_number), color=(-1, -1, -1), height=0.3)
        numberText.draw()
        win.flip()
        core.wait(1.0)

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

    win.color = 'white'
    win.flip()
    if show_faster:
        fasterText.draw()
        win.flip()
    core.wait(0.5)

    win.color = (1, 1, 1)
    win.flip()

# 提问随机数位置并检查回答是否正确
chosen_indices = random.sample(range(len(number_display_indices)), question_count)
correct_answers = [random_numbers[idx] for idx in chosen_indices]

all_correct = True

for idx, correct_answer in zip(chosen_indices, correct_answers):
    attempts_left = max_attempts
    while attempts_left > 0:
        # 使用输入框来获取用户输入
        user_input_dialog = gui.Dlg(title=f"第 {number_display_indices[idx]} 个数字是多少？")
        user_input_dialog.addField("请输入一个数字 (0-9):")
        user_input_dialog.show()

        if not user_input_dialog.OK:  # 如果用户点击了取消或关闭
            win.close()
            core.quit()

        user_input = user_input_dialog.data[0]

        if user_input.isdigit() and int(user_input) == correct_answer:
            break
        else:
            attempts_left -= 1
            if attempts_left > 0:
                errorText = visual.TextStim(
                    win,
                    text=f"输入错误！你还有 {attempts_left} 次机会。",
                    color='red',
                    height=0.1
                )
                errorText.draw()
                win.flip()
                core.wait(1.5)
            else:
                all_correct = False
                finalErrorText = visual.TextStim(
                    win,
                    text="输入错误次数已用完，实验无效。",
                    color='red',
                    height=0.1
                )
                finalErrorText.draw()
                win.flip()
                core.wait(3.0)
                break

    if not all_correct:
        break

# 如果用户回答正确，则保存数据
if all_correct:
    df = pd.DataFrame(dataRecords)
    df.to_csv(filename, index=False)
else:
    print("实验无效，数据未保存。")

win.close()
core.quit()
