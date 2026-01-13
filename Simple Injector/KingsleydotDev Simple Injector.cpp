#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

DWORD GetProcID(const wchar_t* procName)
{
    DWORD procID = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32W procEntry;
        procEntry.dwSize = sizeof(procEntry);
        if (Process32FirstW(hSnap, &procEntry))
        {
            do
            {
                if (!_wcsicmp(procEntry.szExeFile, procName))
                {
                    procID = procEntry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(hSnap, &procEntry));
        }
    }
    CloseHandle(hSnap);
    return procID;
}

std::string GetDLLPath()
{
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::string path = exePath;
    size_t pos = path.find_last_of("\\/");
    if (pos != std::string::npos)
        path = path.substr(0, pos);

    path += "\\Vampire Survivors X.dll";
    return path;
}

int main()
{
    std::string dllPath = GetDLLPath();
    const char* dllPathC = dllPath.c_str();
    const wchar_t* procName = L"VampireSurvivors.exe";

    DWORD procID = 0;

    std::cout << "Waiting for process..." << std::endl;
    while (procID == 0)
    {
        procID = GetProcID(procName);
        Sleep(50);
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);

    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        LPVOID loc = VirtualAllocEx(hProc, nullptr, strlen(dllPathC) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (loc)
        {
            WriteProcessMemory(hProc, loc, dllPathC, strlen(dllPathC) + 1, nullptr);
            FARPROC loadLibAddr = GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
            HANDLE hThread = CreateRemoteThread(hProc, nullptr, 0, (LPTHREAD_START_ROUTINE)loadLibAddr, loc, 0, nullptr);

            if (hThread)
            {
                std::cout << "Successfully injected!" << std::endl;
                CloseHandle(hThread);
            }
        }
    }

    if (hProc) CloseHandle(hProc);
    return 0;
}