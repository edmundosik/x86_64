
OSNAME = TestOS

GNUEFI = efi/gnu-efi
OVMFDIR = boot/OVMFbin
LDS = linker.ld
CC = gcc
LD = ld
ASM = nasm

CFLAGS = -ffreestanding -fshort-wchar -I $(INCLUDEDIR)/
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib
ASMFLAGS = 

SRCDIR := ./
OBJDIR := compiled
BUILDDIR = iso
INCLUDEDIR = include
LIBDIR = lib
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)        
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)   
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%.asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)

all: kernel buildimg

kernel: $(OBJS) link

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo !=== Compiling $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%.asm.o: $(SRCDIR)/%.asm
	@ echo !=== Compiling $^
	@ mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) $^ -f elf64 -o $@
	
link: 
	@ echo !=== Linking 
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)
	
	
setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

buildimg:
	dd if=/dev/zero of=$(OSNAME).img bs=512 count=93750
	mformat -i $(OSNAME).img -f 1440 ::
	mmd -i $(OSNAME).img ::/EFI
	mmd -i $(OSNAME).img ::/EFI/BOOT
	mcopy -i $(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(OSNAME).img $(BUILDDIR)/startup.nsh ::
	mcopy -i $(OSNAME).img $(BUILDDIR)/kernel.elf ::
	mcopy -i $(OSNAME).img $(BUILDDIR)/zap-light16.psf ::

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none

bootloader:
	make -C $(GNUEFI) bootloader