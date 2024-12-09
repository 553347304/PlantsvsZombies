#pragma once

CWnd* ui[1000];
CWnd* MFC;
int ui_Length = sizeof(ui) / sizeof(ui[0]);
int ui_length = ui_Length - 100;
int ui_key[5];

int ui_lv;				// 等级检测
int UI_KEYCODE[4];		// 快捷键
string UI_KEYCODESTR[4];// 快捷键
bool lv_Bool = false;	// 等级检测
int ui_zh;				// 账号数量

string loginfo;

enum Animal {
	Button,
	Edit,
	ComboBox
};
class UI_ { //  : public CJHDlg
private:
	CWnd* ID;
public:
	template <typename T>
	T* P() { return (T*)this->ID; }
	struct UIValue {
		UINT ID;	 // ID
		string V;	 // value
		string Name; // name
		int I;		 // int
		bool B;		 // bool
		bool Enabled;// Enabled
		friend ostream& operator<<(ostream& os, const UIValue& value) {
			os << "ID: " << value.ID << endl;
			os << "value: " << value.V << endl;
			os << "name: " << value.Name << endl;
			os << "int: " << value.I << endl;
			os << "bool: " << value.B << endl;
			os << "Enabled: " << value.Enabled << endl;
			return os;
		}
	};
	int GetType(UINT UID) {
		this->ID = MFC->GetDlgItem(UID);
		if (this->ID == nullptr) { return -1; }
		TCHAR className[256];
		::GetClassName(this->ID->GetSafeHwnd(), className, 256);
		string name = string(CStringA((CString(className))));
		if (name == "Button") { return Button; }
		if (name == "Edit") { return Edit; }
		if (name == "ComboBox") { return ComboBox; }
		return -1;
	}
	UIValue GetValue(UINT UID) {
		CString str;
		string res;
		bool b;
		bool e;
		int num;
		switch (this->GetType(UID)) {
		case Edit:
			this->P<CEdit>()->GetWindowText(str);
			res = CStringA(str).TrimRight();
			//num = AT.FindInt(res);
			return { UID, res, res, 0, NULL, NULL };
		case Button:
			b = this->P<CButton>()->GetCheck();
			e = this->P<CButton>()->IsWindowEnabled();
			if (b) { return { UID, "true", "true", 1, b, e }; }
			return { UID, "false", "false", 0, b, e };
		case ComboBox:
			num = this->P<CComboBox>()->GetCurSel();
			this->P<CComboBox>()->GetLBText(num, str);
			res = to_string(num);
			return { UID, res, string(CStringA((CString(str)))), num, NULL, NULL };
		}
		return {};
	}
	void SetValue(UINT UID, string T) {
		int i = 0;
		if (T == "true") { i = 1; }
		switch (this->GetType(UID)) {
		case Edit: this->P<CEdit>()->SetWindowText(CString(T.c_str())); break;
		case Button: this->P<CButton>()->SetCheck(i); break;			// 是否选中
		case ComboBox: this->P<CComboBox>()->SetCurSel(stoi(T)); break; // 索引
		}
	}


	void Show(CWnd* Box, bool Bool) {
		if (Box != nullptr) {
			if (Bool) { Box->ShowWindow(SW_SHOW); }
			if (!Bool) { Box->ShowWindow(SW_HIDE); }
		}
	}
	void ShowAll(bool Bool) {
		for (int i = 200; i < ui_length; i++) {
			if (i != 461 && i != 462 && i != 463 && i != 464 && i != 465) {
				this->Show(ui[i], Bool);
			}
		}
	}
	void Password(CWnd* Box, bool Bool = true) {
		CEdit* pEdit = (CEdit*)Box;
		pEdit->SetPasswordChar(FALSE);
		if (Bool) {
			pEdit->SetPasswordChar(L'\u2022'); // 设置为圆点字符
			//pEdit->SetPasswordChar('*'); // 设置为星号字符
		}
	}
	// Get
	string GETType(CWnd* pWnd) {
		if (pWnd != nullptr) {
			string UI_str;
			TCHAR className[256];
			::GetClassName(pWnd->GetSafeHwnd(), className, 256);
			if (CString(className) == _T("Button")) {
				return "Button";
			}
			if (CString(className) == _T("Edit")) {
				return "Edit";
			}
			if (CString(className) == _T("ComboBox")) {
				return "ComboBox";
			}
			// return "未知类型";
		}
		return "";
	}
	string GETEdit(CWnd* Box) {
		CEdit* pEdit = (CEdit*)Box;
		CString str;
		pEdit->GetWindowText(str);
		CStringA strA(str);
		return string(strA);
	}
	bool GETCheck(CWnd* Box) {
		CButton* pEdit = (CButton*)Box;
		BOOL isChecked = pEdit->GetCheck();
		if (isChecked) {
			return true;
		}
		return false;
	}
	int GETCombo(CWnd* Box) {
		CComboBox* pEdit = (CComboBox*)Box;
		return pEdit->GetCurSel();  // 下拉框当前选择索引
	}
	// Add
	static void ADDEdit(CWnd* Box, string Text) {
		CEdit* pEdit = (CEdit*)Box;
		if (pEdit != nullptr) {
			pEdit->SetWindowText(CString(Text.c_str()));
		}
	}
	static void ADDCheck(CWnd* Box, bool BoxBool = true) {
		CButton* pEdit = (CButton*)Box;
		if (pEdit != nullptr) {
			pEdit->SetCheck(BST_UNCHECKED);  // 取消选中
			if (BoxBool) {  // 选中按钮
				pEdit->SetCheck(BST_CHECKED);
			}
		}
	}
	static void ADDCombo(CWnd* Box, vector<string> Text = {}, int s = 0, string Head = "") {
		CComboBox* pEdit = (CComboBox*)Box;
		if (pEdit != nullptr) {
			if (!Text.empty()) {
				pEdit->ResetContent();  // 清空下拉框
				// 添加下拉框选项
				size_t length = end(Text) - begin(Text);
				for (size_t i = 0; i < length; i++) {
					pEdit->AddString(CString(Head.c_str()) + CString(Text[i].c_str()));
				}
			}
			pEdit->SetCurSel(s); // 设置默认选中项
		}
	}

	static void ButtonClickKey(CWnd* ID, int vk) {
		if (GetAsyncKeyState(vk) & 0x8000) {
			ID->SendMessage(BM_CLICK, 0, 0);
		}
	}
	static bool ButtonClick(HWND calcWnd, string Title) {
		HWND Button = FindWindowExA(calcWnd, NULL, "Button", Title.c_str());
		if (Button != NULL) {
			SendMessage(Button, BM_CLICK, 0, 0);
			Sleep(300);
			return true;
		}
		return false;
	}
	static bool EditUpdate(HWND calcWnd, string Title, string EditText, string calTitle = "Edit") {
		HWND EditBox = FindWindowExA(calcWnd, NULL, calTitle.c_str(), Title.c_str());
		cout << EditBox << endl;
		if (EditBox != NULL) {
			// 获取编辑框的文本内容
			const int bufferSize = 256; // 假设最大长度为256个字符
			char buffer[bufferSize];
			SendMessageA(EditBox, WM_GETTEXT, bufferSize, (LPARAM)buffer);
			// 修改内容
			if (buffer != EditText) {
				SendMessageA(EditBox, WM_SETTEXT, 0, (LPARAM)EditText.c_str());
			}
			//cout << buffer << endl;
			return true;
		}
		return false;
	}

	static void MsgBox(string Text, string err) {
		MessageBox(NULL, CString(Text.c_str()), CString(err.c_str()), MB_OK | MB_ICONINFORMATION);
	}
	//AfxMessageBox("");
	/*
	//UI ui;
	//ui.Mess(IDC_Time[0], 2);  // 限制输入数量
	void Mess(CEdit* nEditControlID, int num) {
		CString strText;
		CString message;
		nEditControlID->GetWindowText(strText);
		if (strText.GetLength() > num) {
			// 限制输入数量
			message.Format(_T("只能输入最多 %d 位数，请重新输入！"), num);
			MessageBox(message, _T("输入错误"), MB_OK | MB_ICONERROR);
			strText = strText.Left(num); // 截取前两位字符
			nEditControlID->SetWindowText(strText);
		}
	}
	*/
};
UI_ UI;


void CoutPrint(string EditText, bool Bool = true) {
	CString Logstr;
	string message;
	string UI_Log = UI.GETEdit(ui[901]);

	message = EditText + "\r\n";
	if (loginfo != message) {
		UI.ADDEdit(ui[901], UI_Log + message);
		ui[901]->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);  // 自动滚动
	}
	loginfo = message;
}
