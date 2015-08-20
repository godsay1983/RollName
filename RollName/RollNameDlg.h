// RollNameDlg.h : header file
//

#pragma once
#include "bcgpvisualctrl.h"
#include <vector>
#include "afxwin.h"

using namespace std;

// CRollNameDlg dialog
class CRollNameDlg : public CBCGPDialog
{
// Construction
public:
	CRollNameDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ROLLNAME_DIALOG };
	enum { ROLL_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	vector<string> names;
	vector<string> rolledNames;
	bool bStart;
	bool bSwitchOn;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CBCGPVisualContainerCtrl m_wndContainer;
	CBCGPTextGaugeImpl* m_pTextIndicator;
	void Repos();
	int Roll(int nMax, double num);
	void ReadNames();
	void AddRolledName(CString name);
	string RollName();
	afx_msg void OnBnClickedButtonRoll();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CButton m_rollBtn;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
