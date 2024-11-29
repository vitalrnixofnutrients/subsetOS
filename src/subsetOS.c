void PutChar(unsigned char variable, unsigned short port) {
	asm volatile ("outb %0, %1" : : "a" (variable), "Nd" (port));
}
unsigned char* subsetShellBufferPointer = (unsigned char*)0x100000000;
unsigned long long* EntryPoint;
void GetChar(unsigned short port) {
	asm volatile ("inb %1, %0" : "=a" (*subsetShellBufferPointer) : "Nd" (port));
}
void JmpToEntryPoint(void) {
	asm volatile ("jmp *%0" : "=m" (EntryPoint));
}
void IncrementallyRecompile(void) {
	subsetShellBufferPointer = (unsigned char*)0x100000000;
}
void ProcessCommand(void) {
	if (*subsetShellBufferPointer == 'r') {
		subsetShellBufferPointer = subsetShellBufferPointer + 1;
		if (*subsetShellBufferPointer == 'u') {
			subsetShellBufferPointer = subsetShellBufferPointer + 1;
			if (*subsetShellBufferPointer == 'n') {
				subsetShellBufferPointer = subsetShellBufferPointer + 1;
				if (*subsetShellBufferPointer == '\0') {
					JmpToEntryPoint();
				} else { IncrementallyRecompile(); }
			} else { IncrementallyRecompile(); }
		} else { IncrementallyRecompile(); }
	} else { IncrementallyRecompile(); }
}
void subsetOSShell(void) {
	while (1) {
		GetChar(0x3f8);
		switch (*subsetShellBufferPointer) {
			case 0x0:
				break;
			case 0x7f:
				if (subsetShellBufferPointer != (unsigned char*)0x100000000) {	// To prevent a buffer underflow, it won't let you backspace an empty buffer
					PutChar('\b', 0x3f8);
					PutChar(' ', 0x3f8);
					PutChar('\b', 0x3f8);
					*subsetShellBufferPointer = '\0';
					subsetShellBufferPointer = subsetShellBufferPointer - 1;
				}
				break;
			case 0xd:
				PutChar('\r', 0x3f8);
				PutChar('\n', 0x3f8);
				*subsetShellBufferPointer = '\0';
				subsetShellBufferPointer = (unsigned char*)0x100000000;
				ProcessCommand();
				for (subsetShellBufferPointer = (unsigned char*)0x100000000; *subsetShellBufferPointer != 0x0; subsetShellBufferPointer = subsetShellBufferPointer + 1) {
					*subsetShellBufferPointer = '\0';
				}
				subsetShellBufferPointer = (unsigned char*)0x100000000;
				break;
			default:
				if (subsetShellBufferPointer != (unsigned char*)0x100000100) {	// To prevent a buffer overflow, it won't let you type more than 256 characters
					PutChar(*subsetShellBufferPointer, 0x3f8);
					subsetShellBufferPointer = subsetShellBufferPointer + 1;
				}
					break;
		}
	}
}
void main(void* EFIImage, void* EFISystemTable) {
	subsetOSShell();
}
