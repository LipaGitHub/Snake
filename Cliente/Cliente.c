#include "..\DLL\DLL.h"


#define PIPE_NAME TEXT("\\\\.\\pipe\\teste")
int _tmain(int argc, LPTSTR argv[]) {
	TCHAR buf[256];
	HANDLE hPipe;
	int i = 0;
	BOOL ret;
	DWORD n;
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif
	_tprintf(TEXT("[Cliente]Esperar pelo pipe '%s'(WaitNamedPipe)\n"), PIPE_NAME);
	if (!WaitNamedPipe(PIPE_NAME, NMPWAIT_WAIT_FOREVER)) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (WaitNamedPipe)\n"), PIPE_NAME);
		exit(-1);
	}
	_tprintf(TEXT("[Cliente] Liga��o ao Servidor... (CreateFile)\n"));
	hPipe = CreateFile(PIPE_NAME, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hPipe == NULL) {
		_tprintf(TEXT("[ERRO] Ligar ao pipe '%s'... (CreateFile)\n"), PIPE_NAME);
		exit(-1);
	}
	_tprintf(TEXT("[Cliente]Liguei-me...\n"));
	while (1) {
		ret = ReadFile(hPipe, buf, sizeof(buf), &n, NULL);
		buf[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n)
			break;
		_tprintf(TEXT("[Cliente] Recebi %d bytes: '%s'... (ReadFile)\n"), n, buf);
	}
	CloseHandle(hPipe);
	Sleep(200);
	return 0;
}