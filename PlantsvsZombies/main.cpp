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

    cerr << "��ȡƫ�Ƶ�ַʧ��: " << hex << addr << endl;
    return 0;
}

void Write(DWORD addr, int v) {

    if (WriteProcessMemory(hd, (LPVOID)addr, &v, sizeof(v), NULL)) {
        cerr << "ע��ɹ�->��ǰ��ֵ: " + to_string(v) << endl;
    }
    else {
        cout << "ע��ʧ��" << endl;
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
    cout << "�ڴ��ַ: " << hex << result << endl;
    cout << "��ǰֵ: " << dec << read << endl;
    Write(result, v);
}



int Main() {
    HWND hwnd = FindWindowA("MainWindow", NULL);//��ȡ���ھ��
    DWORD pid;
    GetWindowThreadProcessId(hwnd, &pid);
    hd = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);  // ����Ȩ��
    if (hd == NULL) {
        cout << "�޷��򿪽��̣��������: " << GetLastError() << endl;
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