
OSNAME = TestOS

GNUEFI = efi/gnu-efi
OVMFDIR = boot/OVMFbin
LDS = linker.ld
CC = gcc
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -I $(INCLUDEDIR)/
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := ./
OBJDIR := compiled
BUILDDIR = iso
INCLUDEDIR = include
LIBDIR = lib
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)          
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
DIRS = $(wildcard $(SRCDIR)/*)

all: kernel buildimg

kernel: $(OBJS) link

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo !=== Compiling $^
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@
	
link: 
	@ echo !=== Linking 
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.exe $(OBJS)
	
	
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
	mcopy -i $(OSNAME).img $(BUILDDIR)/kernel.exe ::
	mcopy -i $(OSNAME).img $(BUILDDIR)/zap-light16.psf ::

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none

bootloader:
	make -C $(GNUEFI) bootloader