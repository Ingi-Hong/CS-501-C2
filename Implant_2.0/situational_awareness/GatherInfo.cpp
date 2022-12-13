#include "GatherInfo.h"


string getComputerName() {
    char buffer[MAX_COMPUTERNAME_LENGTH + 1] = {0};
    DWORD cchBufferSize = sizeof(buffer) / sizeof(buffer[0]);
    if (!GetComputerNameA(buffer, &cchBufferSize))
        throw runtime_error("GetComputerName() failed.");
    return string(&buffer[0]);
}

string getUserName() {
    char buffer[257] = {0};
    DWORD cchBufferSize = sizeof(buffer) / sizeof(buffer[0]);
    if (!GetUserNameA(buffer, &cchBufferSize))
        throw runtime_error("GetUserName() failed.");
    return string(&buffer[0]);
}

//https://stackoverflow.com/questions/56043589/how-to-recover-privileges-with-gettokeninformation-c
BOOL CheckWindowsPrivilege(const TCHAR* Privilege)
{
    LUID luid;
    PRIVILEGE_SET privs;
    HANDLE hProcess;
    HANDLE hToken;
    hProcess = GetCurrentProcess();
    if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) return FALSE;
    if (!LookupPrivilegeValue(NULL, Privilege, &luid)) return FALSE;
    privs.PrivilegeCount = 1;
    privs.Control = PRIVILEGE_SET_ALL_NECESSARY;
    privs.Privilege[0].Luid = luid;
    privs.Privilege[0].Attributes = SE_PRIVILEGE_ENABLED;
    BOOL bResult;
    PrivilegeCheck(hToken, &privs, &bResult);
    return bResult;
}


//https://learn.microsoft.com/zh-cn/windows/win32/api/securitybaseapi/nf-securitybaseapi-checktokenmembership?redirectedfrom=MSDN
BOOL IsUserAdmin()
{
    BOOL b;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    PSID AdministratorsGroup;
    b = AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &AdministratorsGroup);
    if (b)
    {
        if (!CheckTokenMembership(NULL, AdministratorsGroup, &b))
        {
            b = FALSE;
        }
        FreeSid(AdministratorsGroup);
    }

    return(b);
}

vector<pair<string, bool>> checkPrivileges() {
    vector<pair<string, bool>> privileges;

    privileges.push_back(make_pair(string("SE_ASSIGNPRIMARYTOKEN_NAME"), CheckWindowsPrivilege(SE_ASSIGNPRIMARYTOKEN_NAME)));
    privileges.push_back(make_pair(string("SE_INCREASE_QUOTA_NAME"), CheckWindowsPrivilege(SE_INCREASE_QUOTA_NAME)));
    privileges.push_back(make_pair(string("SE_BACKUP_NAME"), CheckWindowsPrivilege(SE_BACKUP_NAME)));
    privileges.push_back(make_pair(string("SE_CHANGE_NOTIFY_NAME"), CheckWindowsPrivilege(SE_CHANGE_NOTIFY_NAME)));
    privileges.push_back(make_pair(string("SE_CREATE_GLOBAL_NAME"), CheckWindowsPrivilege(SE_CREATE_GLOBAL_NAME)));
    privileges.push_back(make_pair(string("SE_CREATE_PAGEFILE_NAME"), CheckWindowsPrivilege(SE_CREATE_PAGEFILE_NAME)));
    privileges.push_back(make_pair(string("SE_CREATE_SYMBOLIC_LINK_NAME"), CheckWindowsPrivilege(SE_CREATE_SYMBOLIC_LINK_NAME)));
    privileges.push_back(make_pair(string("SE_CREATE_TOKEN_NAME"), CheckWindowsPrivilege(SE_CREATE_TOKEN_NAME)));
    privileges.push_back(make_pair(string("SE_DEBUG_NAME"), CheckWindowsPrivilege(SE_DEBUG_NAME)));
    //rivileges.push_back(make_pair(string("SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME"), CheckWindowsPrivilege(SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME)));
    privileges.push_back(make_pair(string("SE_ENABLE_DELEGATION_NAME"), CheckWindowsPrivilege(SE_ENABLE_DELEGATION_NAME)));
    privileges.push_back(make_pair(string("SE_IMPERSONATE_NAME"), CheckWindowsPrivilege(SE_IMPERSONATE_NAME)));
    privileges.push_back(make_pair(string("SE_INC_BASE_PRIORITY_NAME"), CheckWindowsPrivilege(SE_INC_BASE_PRIORITY_NAME)));
    privileges.push_back(make_pair(string("SE_INC_WORKING_SET_NAME"), CheckWindowsPrivilege(SE_INC_WORKING_SET_NAME)));
    privileges.push_back(make_pair(string("SE_LOAD_DRIVER_NAME"), CheckWindowsPrivilege(SE_LOAD_DRIVER_NAME)));
    privileges.push_back(make_pair(string("SE_LOCK_MEMORY_NAME"), CheckWindowsPrivilege(SE_LOCK_MEMORY_NAME)));
    privileges.push_back(make_pair(string("SE_MACHINE_ACCOUNT_NAME"), CheckWindowsPrivilege(SE_MACHINE_ACCOUNT_NAME)));
    privileges.push_back(make_pair(string("SE_MANAGE_VOLUME_NAME"), CheckWindowsPrivilege(SE_MANAGE_VOLUME_NAME)));
    privileges.push_back(make_pair(string("SE_PROF_SINGLE_PROCESS_NAME"), CheckWindowsPrivilege(SE_PROF_SINGLE_PROCESS_NAME)));
    privileges.push_back(make_pair(string("SE_RELABEL_NAME"), CheckWindowsPrivilege(SE_RELABEL_NAME)));
    privileges.push_back(make_pair(string("SE_REMOTE_SHUTDOWN_NAME"), CheckWindowsPrivilege(SE_REMOTE_SHUTDOWN_NAME)));
    privileges.push_back(make_pair(string("SE_RESTORE_NAME"), CheckWindowsPrivilege(SE_RESTORE_NAME)));
    privileges.push_back(make_pair(string("SE_SECURITY_NAME"), CheckWindowsPrivilege(SE_SECURITY_NAME)));
    privileges.push_back(make_pair(string("SE_SHUTDOWN_NAME"), CheckWindowsPrivilege(SE_SHUTDOWN_NAME)));
    privileges.push_back(make_pair(string("SE_SYNC_AGENT_NAME"), CheckWindowsPrivilege(SE_SYNC_AGENT_NAME)));
    privileges.push_back(make_pair(string("SE_SYSTEM_ENVIRONMENT_NAME"), CheckWindowsPrivilege(SE_SYSTEM_ENVIRONMENT_NAME)));
    privileges.push_back(make_pair(string("SE_SYSTEM_PROFILE_NAME"), CheckWindowsPrivilege(SE_SYSTEM_PROFILE_NAME)));
    privileges.push_back(make_pair(string("SE_SYSTEMTIME_NAME"), CheckWindowsPrivilege(SE_SYSTEMTIME_NAME)));
    privileges.push_back(make_pair(string("SE_TAKE_OWNERSHIP_NAME"), CheckWindowsPrivilege(SE_TAKE_OWNERSHIP_NAME)));
    privileges.push_back(make_pair(string("SE_TCB_NAME"), CheckWindowsPrivilege(SE_TCB_NAME)));
    privileges.push_back(make_pair(string("SE_TIME_ZONE_NAME"), CheckWindowsPrivilege(SE_TIME_ZONE_NAME)));
    privileges.push_back(make_pair(string("SE_TRUSTED_CREDMAN_ACCESS_NAME"), CheckWindowsPrivilege(SE_TRUSTED_CREDMAN_ACCESS_NAME)));
    privileges.push_back(make_pair(string("SE_UNDOCK_NAME"), CheckWindowsPrivilege(SE_UNDOCK_NAME)));
    return privileges;
}

json GetAll(){
    json res;
    const string compName = getComputerName();
    const string userName = getUserName();
    cout << compName << endl;
    cout << userName << endl;
    //cout << IsUserAnAdmin() << endl;
    vector<pair<string, bool>> p( checkPrivileges());
    /*
    for(pair<string, bool> p : checkPrivileges()) {
        cout << p.first << ": " << p.second << endl;
    }
    */
    res["compName"] = compName;
    res["userName"] = userName;
    res["Privileges"] = p;
    return res;

}



/*


NEED TO ADD IN MORE FUNCTIONS HERE


*/
// int main() {
//     GetAll();
// }

