#include "..\DLL\DLL.h"
#define N 5
HANDLE hPipe[N];
DWORD WINAPI RecebeClientes(LPVOID p);
int total =0 ;

#define PIPE_NAME TEXT("\\\\.\\pipe\\teste")

int _tmain(int argc, LPTSTR argv[]) {
#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif
	
	TCHAR buf[256];
	DWORD n = 0;
	

	//Preencher array de Handle com invalid_handle_value
	for (int i = 0; i < N; i++)
		hPipe[i] = INVALID_HANDLE_VALUE;

	//Criação da Thread para cada cliente
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RecebeClientes, NULL, 0, NULL);

	do {
		_tprintf(TEXT("[SERVIDOR] Entre com o comando:"));
		_fgetts(buf, 256, stdin);

		for (int i = 0; i < N; i++) {
			if (hPipe[i] != INVALID_HANDLE_VALUE)
				if (!WriteFile(hPipe[i], buf, _tcslen(buf)*sizeof(TCHAR), &n, NULL)) {
					_tprintf(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
					//disconnect  namedpipe();
					DisconnectNamedPipe(hPipe[i]);
					//Closehandle()M
					CloseHandle(hPipe[i]);
					//preencher com invalid_handle_value
					hPipe[i] = INVALID_HANDLE_VALUE;
					total--;
				}
		}
		_tprintf(TEXT("[SERVIDOR] Enviei %d bytes aos %d Clientes... (WriteFile)\n"), n, total);
	} while (_tcsncmp(buf, TEXT("fim"), 3));
	exit(0);
}

DWORD WINAPI RecebeClientes(LPVOID p) {
	TCHAR buf[256];
	int indice, i;
	HANDLE pipe_temp;

	while (1) {
		if (total < N) { 
			pipe_temp = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_OUTBOUND, PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, N, sizeof(buf), sizeof(buf), 1000, NULL);
			if (pipe_temp == INVALID_HANDLE_VALUE) {
				_tperror(TEXT("Erro na ligação ao Cliente!"));
				exit(-1);
			}
			if (!ConnectNamedPipe(pipe_temp, NULL)) {
				_tprintf(TEXT("Erro na ligação ao Cliente!"));
				exit(-1);
			}
		}
		else {
			Sleep(1000); 
			continue;
		}
		for (i = 0; i < N; i++)  
			if (hPipe[i] == INVALID_HANDLE_VALUE) { 
				hPipe[i] = pipe_temp;
				total++;
				break; 
			}

	}
	return 0;
}
