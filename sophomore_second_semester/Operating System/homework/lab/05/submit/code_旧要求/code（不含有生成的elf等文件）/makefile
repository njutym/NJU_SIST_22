# # 运行命令: make即可
# # 反汇编指令：objdump -D  -Mi8086,suffix app.elf
# all:app.bin

app.bin:app.elf
	objcopy -R.eh_frame \
		-R \
		.comment \
		-O \
		binary \
		app.elf app.bin
	make qemu

# 为了让被执行的第一个函数是main，用-e main
app.elf:main.o api.o 
	ld -m \
	elf_i386 \
	-T link.ld \
	-e main \
	main.o api.o \
	-o app.elf 

# 要用-m16
main.o:main.c myos.h
	gcc -m16 \
		-ffreestanding \
		-fno-pic \
		-I. \
		-c main.c \
		-o main.o

api.o:api.asm
	nasm -f elf32 api.asm -o api.o

qemu:
	qemu-img create -f raw PA5_disk.raw 5G
	nasm -f bin -o kernel.bin kernel.asm
	make dd
	
dd:
	dd if=kernel.bin of=PA5_disk.raw bs=512 count=1
	dd if=app.bin of=PA5_disk.raw bs=512 seek=1 count=1
# dd 命令中的 seek=1 用于将写入光标定位到磁盘映像文件(PA5_disk.raw)的第1个扇区，然后再将来自 app.bin 的数据写入。
# 实现从第二个扇区开始写入 app.bin 的数据，而不是覆盖现有数据。
	make run

run:
	qemu-system-x86_64 -drive file=PA5_disk.raw,format=raw

clear:
	rm api.o app.bin app.elf kernel.bin main.o


