all:
	@gcc -ffreestanding -fshort-wchar -no-pie -c Source/main.cpp -o Build/main.o
	@ld -T linker.ld -static -Bsymbolic -nostdlib -o ../Build/ISO/inferno Build/main.o