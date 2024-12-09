#include<Windows.h>
#include<stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <psapi.h>
using namespace std;

HANDLE hd;

DWORD Read(DWORD addr) {
    DWORD buffer;
    SIZE_T bytes_read;
    if (ReadProcessMemory(hd, (LPCVOID)addr, &buffer, sizeof(buffer), &bytes_read)) {
        return buffer;
    }

    cerr << "读取偏移地址失败: " << hex << addr << endl;
    return 0;
}

void Write(DWORD addr, int v) {

    if (WriteProcessMemory(hd, (LPVOID)addr, &v, sizeof(v), NULL)) {
        cerr << "注入成功->当前数值: " + to_string(v) << endl;
    }
    else {
        cout << "注入失败" << endl;
    }

    CloseHandle(hd);
}

void Ram(vector<DWORD>& arr, int v) {
    DWORD read = 0;
    DWORD result = 0x0;

    for (int i = 0; i < arr.size(); ++i) {
        result = read + arr[i];
        read = Read(result);
    }
    cout << "内存地址: " << hex << result << endl;
    cout << "当前值: " << dec << read << endl;
    Write(result, v);
}



int Main() {
    HWND hwnd = FindWindowA("MainWindow", NULL);//获取窗口句柄
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    hd = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);  // 提升权限
    if (hd == NULL) {
        cout << "无法打开进程，错误代码: " << GetLastError() << endl;
        return 1;
    }

    vector<DWORD> arr = {
        0x400000 + 0x2A9EC0,
        0x768,
        0x5560,
    };
    Ram(arr, 300);

    return 0;
}