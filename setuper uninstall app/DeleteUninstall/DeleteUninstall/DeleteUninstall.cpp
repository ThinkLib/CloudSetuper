// DeleteUninstall.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DeleteUninstall.h"
#include "Shellapi.h"
#include "winbase.h"
#include "shlobj.h"

#define MAX_LOADSTRING 100

// Global Variables:

// Forward declarations of functions included in this code module:
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// ɾ��uninstall.exe(���ְ�)
	int n=200;
	if (strlen(lpCmdLine)){
		do {
			if (DeleteFile(lpCmdLine)){
				break;
			}
			else{
				int nErrorCode=GetLastError();
				if (ERROR_FILE_NOT_FOUND == nErrorCode)
					break;
			}
			Sleep(10);
			n--;
		} while (n > 0);
	}

	SHELLEXECUTEINFO sei;  
	TCHAR szModule [MAX_PATH],szComspec[MAX_PATH],szParams [MAX_PATH];  
	//��ȡ�ļ�·����  
	if((GetModuleFileName(0,szModule,MAX_PATH)!=0) &&  
		(GetShortPathName(szModule,szModule,MAX_PATH)!=0) &&  
		(GetEnvironmentVariable("COMSPEC",szComspec,MAX_PATH)!=0))  
	{  //���������в�����  
		lstrcpy(szParams,"/c del ");  
		lstrcat(szParams, szModule);  
		lstrcat(szParams, " > nul");  

		//��ʼ��SHELLEXECUTEINFO�ṹ��Ա  
		sei.cbSize = sizeof(sei);//�������ʹ�С��  
		//����ڽ��̾����ShellExecuteEx����ִ��ʱ���á�  
		sei.hwnd = 0;  
		sei.lpVerb = "Open";//ִ�ж���Ϊ����ִ�С���  
		sei.lpFile = szComspec;      //ִ�г����ļ�ȫ·�����ơ�  
		sei.lpParameters = szParams; //ִ�в�����  
		sei.lpDirectory = 0;  
		//��ʾ��ʽ���˴�ʹ�����ط�ʽ��ֹ��������ڽ��档  
		sei.nShow = SW_HIDE;  
		//����ΪSellExecuteEx��������������˳���  
		sei.fMask = SEE_MASK_NOCLOSEPROCESS;  
		//����ִ������ڽ��̡�  
		if(ShellExecuteEx(&sei))  
		{  //���������н��̵�ִ�м���Ϊ����ִ��,��ʹ���������㹻��ʱ����ڴ����˳���    
			SetPriorityClass(sei.hProcess,IDLE_PRIORITY_CLASS);  
			//���ñ�������̵�ִ�м���Ϊʵʱִ�У��Ȿ�������ϻ�ȡCPUִ��Ȩ�������˳���    
			SetPriorityClass(GetCurrentProcess(),REALTIME_PRIORITY_CLASS);  
			SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);    
			//֪ͨWindows��Դ��������������ļ��Ѿ���ɾ����  
			SHChangeNotify(SHCNE_DELETE,SHCNF_PATH,szModule,0);  
			//ִ���˳�����  
			//EndDialog(0);  
		} 
	}
}



