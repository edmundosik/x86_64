set OSNAME=TestOS
set OVMFDIR=%0/../efi/OVMFbin
qemu-system-x86_64 -machine q35 -cdrom %OSNAME%.iso -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="%OVMFDIR%/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="%OVMFDIR%/OVMF_VARS-pure-efi.fd" -net none
pause
