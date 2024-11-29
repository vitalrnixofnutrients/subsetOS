.DEFAULT_GOAL: all
.SILENT: all clean qemu
all:
	cd build && \
	rm -rf BOOTX64.EFI subsetOS.img && \
	dd if=/dev/zero of=subsetOS.img bs=64M count=1 && \
	mkfs.fat -F 32 subsetOS.img && \
	mmd -i subsetOS.img ::/EFI && \
	mmd -i subsetOS.img ::/EFI/BOOT && \
	clang -target x86_64-unknown-windows -ffreestanding -mno-red-zone -nostdlib -Wl,-entry:main -Wl,-subsystem:efi_application -fuse-ld=lld-link -Os -m64 -o BOOTX64.EFI ../src/subsetOS.c && \
	mcopy -i subsetOS.img BOOTX64.EFI ::/EFI/BOOT

clean:
	rm -rf build/*

qemu:
	qemu-system-x86_64 -smp 12 -m 8G -enable-kvm -nographic -serial mon:stdio -drive format=raw,if=pflash,unit=0,readonly=on,file=/usr/share/OVMF/x64/OVMF_CODE.4m.fd -drive format=raw,index=1,media=disk,if=virtio,file=build/subsetOS.img

.PHONY:
	all clean qemu
