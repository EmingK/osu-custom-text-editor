/*\
|*| custom-runner
|*|
|*| run a custom app and pass original args to the app's first arg
|*| designed for customize osu! "open in notepad" editor
|*|
|*| Author: EmingK <EMKCentric@gmail.com>
\*/

#include <windows.h>

char const *appName = "CommandRunner";

char *parseCmdLine(char const *cmdLine);
char *getCustomApp();
void runAppWithArgs(char const *app, char const *args);

void start() {
  char const *cmdLine = GetCommandLine();
  char *args = parseCmdLine(cmdLine);

  char *app = getCustomApp();

  runAppWithArgs(app, args);

  LocalFree(args);
  LocalFree(app);
  ExitProcess(0);
}

void error(char const *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char *message = LocalAlloc(LPTR, MAX_PATH + 100);
  wvsprintf(message, fmt, args);
  MessageBox(NULL, message, appName, MB_OK);
  LocalFree(message);
  va_end(args);
  ExitProcess(-1);
}

void runAppWithArgs(char const *app, char const *args) {
  char *fullCmdLine = LocalAlloc(LPTR, MAX_PATH);
  if (!fullCmdLine) {
    error("内存不足");
  }

  if (lstrlen(args)) {
    wsprintf(fullCmdLine, "%s \"%s\"", app, args);
  } else {
    wsprintf(fullCmdLine, "%s", app);
  }
  
  STARTUPINFO si = { sizeof(STARTUPINFO) };
  PROCESS_INFORMATION pi = {};
  BOOL processResult = CreateProcess(NULL,
				     fullCmdLine,
				     NULL,
				     NULL,
				     FALSE,
				     NORMAL_PRIORITY_CLASS,
				     NULL,
				     NULL,
				     &si,
				     &pi);
  if (!processResult) {
    error("运行外部程序 %s 失败", fullCmdLine);
  }
  
  CloseHandle(pi.hThread);
  LocalFree(fullCmdLine);
}

char *parseCmdLine(char const *cmdLine) {
  int cmdLength = lstrlen(cmdLine);
  BOOL arg0HasQuote = cmdLine[0] == '\"';
  char charToFind = ' ';
  if (arg0HasQuote) {
    charToFind = '\"';
  }

  int i = 0;
  while (i < cmdLength) {
    ++i;
    if (cmdLine[i] == charToFind) {
      break;
    }
  }

  if (arg0HasQuote) {
    i += 2;
  } else {
    ++i;
  }

  char *argv = LocalAlloc(LPTR, cmdLength - i + 1);
  lstrcpy(argv, cmdLine + i);
  
  return argv;
}

char *getCustomApp() {
  HANDLE hFile = CreateFile("customrunner.txt",
			    GENERIC_READ,
			    FILE_SHARE_READ,
			    NULL,
			    OPEN_EXISTING,
			    FILE_ATTRIBUTE_NORMAL,
			    NULL);
  if (!hFile) {
    error("读取配置文件失败，确认customrunner.txt文件存在");
  }

  char *fileBuf = LocalAlloc(LPTR, MAX_PATH);
  unsigned long bytesRead = 0;
  ReadFile(hFile, fileBuf, MAX_PATH - 1, &bytesRead, NULL);
  if (bytesRead == 0) {
    CloseHandle(hFile);
    error("读取配置文件失败，确认customrunner.txt文件有内容");
  }

  // only load first line as command
  unsigned long i = 0;
  while (i < bytesRead) {
    if (fileBuf[i] == '\r' || fileBuf[i] == '\n') {
      fileBuf[i] = 0;
      break;
    }
    ++i;
  }

  CloseHandle(hFile);
  return fileBuf;
}
