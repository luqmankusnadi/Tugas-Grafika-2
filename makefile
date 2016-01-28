all:
	gcc -o exec main.c bitmap.c framebuffer.c
    
run:
	sudo ./exec
