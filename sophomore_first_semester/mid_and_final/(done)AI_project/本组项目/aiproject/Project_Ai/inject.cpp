#include <cstdio>
#include <Windows.h>
#include <TlHelp32.h>

#define PROCESS_NAME "th123_beta.exe" // 要注入的进程名，这个是任务管理器的进程名
#define DLL_NAME "speed_hack.dll"     // 要注入的DLL的名称

BOOL InjectDll(DWORD dwPid, CHAR szDllName[]);                   // 注入DLL
DWORD GetPID(PCHAR pProName);                                    // 根据进程名获取PID
VOID ShowError(PCHAR msg);                                       // 打印错误信息
BOOL EnbalePrivileges(HANDLE hProcess, char *pszPrivilegesName); // 提升进程权限

int main()
{
    CHAR szDllPath[MAX_PATH] = {0}; // 保存要注入的DLL的路径
    DWORD dwPID = 0;                // 保存要注入的进程的PID

    // 提升当前进程令牌权限
    if (!EnbalePrivileges(GetCurrentProcess(), SE_DEBUG_NAME))
    {
        printf("权限提升失败\n");
    }

    dwPID = GetPID(PROCESS_NAME);
    if (dwPID == 0)
    {
        printf("没有找到要注入的进程\n");
        goto exit;
    }

    GetCurrentDirectory(MAX_PATH, szDllPath); // 获取程序的目录
    strcat(szDllPath, "\\");
    strcat(szDllPath, DLL_NAME); // 与DLL名字拼接得到DLL的完整路径
    printf("要注入的进程名:%s PID:%d\n", PROCESS_NAME, dwPID);
    printf("要注入的DLL的完整路径%s\n", szDllPath);

    if (InjectDll(dwPID, szDllPath))
    {
        printf("Dll注入成功\n");
    }
exit:
    system("pause");

    return 0;
}

BOOL InjectDll(DWORD dwPid, CHAR szDllName[])
{
    BOOL bRet = TRUE;

    return bRet;
}

DWORD GetPID(PCHAR pProName)
{
    PROCESSENTRY32 pe32 = {0};
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL bRet = FALSE;
    DWORD dwPID = 0;

    if (hSnap == INVALID_HANDLE_VALUE)
    {
        printf("CreateToolhelp32Snapshot process %d\n", GetLastError());
        goto exit;
    }

    pe32.dwSize = sizeof(pe32);
    bRet = Process32First(hSnap, &pe32);
    while (bRet)
    {
        if (lstrcmp(pe32.szExeFile, pProName) == 0)
        {
            dwPID = pe32.th32ProcessID;
            break;
        }
        bRet = Process32Next(hSnap, &pe32);
    }

    CloseHandle(hSnap);
exit:
    return dwPID;
}

VOID ShowError(PCHAR msg)
{
    printf("%s Error %d\n", msg, GetLastError());
}

BOOL EnbalePrivileges(HANDLE hProcess, char *pszPrivilegesName)
{
    HANDLE hToken = NULL;
    LUID luidValue = {0};
    TOKEN_PRIVILEGES tokenPrivileges = {0};
    BOOL bRet = FALSE;
    DWORD dwRet = 0;

    // 打开进程令牌并获取具有 TOKEN_ADJUST_PRIVILEGES 权限的进程令牌句柄
    if (!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        ShowError("OpenProcessToken");
        goto exit;
    }

    // 获取本地系统的 pszPrivilegesName 特权的LUID值
    if (!LookupPrivilegeValue(NULL, pszPrivilegesName, &luidValue))
    {
        ShowError("LookupPrivilegeValue");
        goto exit;
    }

    // 设置提升权限信息
    tokenPrivileges.PrivilegeCount = 1;
    tokenPrivileges.Privileges[0].Luid = luidValue;
    tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    // 提升进程令牌访问权限
    if (!AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, 0, NULL, NULL))
    {
        ShowError("AdjustTokenPrivileges");
        goto exit;
    }
    else
    {
        // 根据错误码判断是否特权都设置成功
        dwRet = ::GetLastError();
        if (ERROR_SUCCESS == dwRet)
        {
            bRet = TRUE;
            goto exit;
        }
        else if (ERROR_NOT_ALL_ASSIGNED == dwRet)
        {
            ShowError("ERROR_NOT_ALL_ASSIGNED");
            goto exit;
        }
    }
exit:
    return bRet;
}