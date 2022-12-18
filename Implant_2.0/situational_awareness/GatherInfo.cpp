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

std::string checkPrivileges() {
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
    //privileges.push_back(make_pair(string("SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME"), CheckWindowsPrivilege(SE_DELEGATE_SESSION_USER_IMPERSONATE_NAME)));
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
    std::string result = "";
    for (auto p : privileges) {
        result += "\"" + p.first + "\":";
        if (p.second) result += "\"true\",";
        else result += "\"false\",";
    }
    return result;
}

std::string getIP(){
    std::string results;
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof (IP_ADAPTER_INFO));

    GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);

    GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
        pAdapter = pAdapterInfo;
        while (pAdapter) {
            std::string test = pAdapter->IpAddressList.IpAddress.String;
            if(test.compare("0.0.0.0")==0){
                
            }else{
                //results.push_back(pAdapter->IpAddressList.IpAddress.String);
                results = pAdapter->IpAddressList.IpAddress.String;
            }
            pAdapter = pAdapter->Next;
            
        }
    if (pAdapterInfo)
        free(pAdapterInfo);
    return results;
}

std::string getInterface(){
  
    // Declare and initialize variables.

    DWORD dwSize = 0;
    DWORD dwRetVal = 0;

    unsigned int i, j;

    /* variables used for GetIfTable and GetIfEntry */
    MIB_IFTABLE *pIfTable;
    MIB_IFROW *pIfRow;

    // Allocate memory for our pointers.
    pIfTable = (MIB_IFTABLE *) malloc(sizeof (MIB_IFTABLE));
    if (pIfTable == NULL) {
        printf("Error allocating memory needed to call GetIfTable\n");
       // return 1;
    }
    // Make an initial call to GetIfTable to get the
    // necessary size into dwSize
    dwSize = sizeof (MIB_IFTABLE);
    if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
        free(pIfTable);
        pIfTable = (MIB_IFTABLE *) malloc(dwSize);
        if (pIfTable == NULL) {
            printf("Error allocating memory needed to call GetIfTable\n");
           // return 1;
        }
    }
    // Make a second call to GetIfTable to get the actual
    // data we want.
    if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
        printf("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
        for (i = 0; i < pIfTable->dwNumEntries; i++) {
            pIfRow = (MIB_IFROW *) & pIfTable->table[i];
            printf("\tIndex[%d]:\t %ld\n", i, pIfRow->dwIndex);
            printf("\tInterfaceName[%d]:\t %ws", i, pIfRow->wszName);
            printf("\n");
            printf("\tDescription[%d]:\t ", i);
            for (j = 0; j < pIfRow->dwDescrLen; j++)
                printf("%c", pIfRow->bDescr[j]);
            printf("\n");
            printf("\tType[%d]:\t ", i);
            switch (pIfRow->dwType) {
            case IF_TYPE_OTHER:
                printf("Other\n");
                break;
            case IF_TYPE_ETHERNET_CSMACD:
                printf("Ethernet\n");
                break;
            case IF_TYPE_ISO88025_TOKENRING:
                printf("Token Ring\n");
                break;
            case IF_TYPE_PPP:
                printf("PPP\n");
                break;
            case IF_TYPE_SOFTWARE_LOOPBACK:
                printf("Software Lookback\n");
                break;
            case IF_TYPE_ATM:
                printf("ATM\n");
                break;
            case IF_TYPE_IEEE80211:
                printf("IEEE 802.11 Wireless\n");
                break;
            case IF_TYPE_TUNNEL:
                printf("Tunnel type encapsulation\n");
                break;
            case IF_TYPE_IEEE1394:
                printf("IEEE 1394 Firewire\n");
                break;
            default:
                printf("Unknown type %ld\n", pIfRow->dwType);
                break;
            }
            printf("\tMtu[%d]:\t\t %ld\n", i, pIfRow->dwMtu);
            printf("\tSpeed[%d]:\t %ld\n", i, pIfRow->dwSpeed);
            printf("\tPhysical Addr:\t ");
            if (pIfRow->dwPhysAddrLen == 0)
                printf("\n");
            for (j = 0; j < pIfRow->dwPhysAddrLen; j++) {
                if (j == (pIfRow->dwPhysAddrLen - 1))
                    printf("%.2X\n", (int) pIfRow->bPhysAddr[j]);
                else
                    printf("%.2X-", (int) pIfRow->bPhysAddr[j]);
            }
            printf("\tAdmin Status[%d]:\t %ld\n", i, pIfRow->dwAdminStatus);
            printf("\tOper Status[%d]:\t ", i);
            switch (pIfRow->dwOperStatus) {
            case IF_OPER_STATUS_NON_OPERATIONAL:
                printf("Non Operational\n");
                break;
            case IF_OPER_STATUS_UNREACHABLE:
                printf("Unreachable\n");
                break;
            case IF_OPER_STATUS_DISCONNECTED:
                printf("Disconnected\n");
                break;
            case IF_OPER_STATUS_CONNECTING:
                printf("Connecting\n");
                break;
            case IF_OPER_STATUS_CONNECTED:
                printf("Connected\n");
                break;
            case IF_OPER_STATUS_OPERATIONAL:
                printf("Operational\n");
                break;
            default:
                printf("Unknown status %ld\n", pIfRow->dwAdminStatus);
                break;
            }
            printf("\n");
        }
    } else {
        printf("GetIfTable failed with error: \n", dwRetVal);
        if (pIfTable != NULL) {
            free(pIfTable);
            pIfTable = NULL;
        }  
        //return 1;
        // Here you can use FormatMessage to find out why 
        // it failed.
    }
    if (pIfTable != NULL) {
        free(pIfTable);
        pIfTable = NULL;
    }
}

json GetAll(){
    json res;
    const string compName = getComputerName();
    const string userName = getUserName();
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
    res["compName"] = compName;
    res["userName"] = userName;
    res["Privileges"] = p;
    //res["ipName"] = ipName;
    return res;

}



/*


NEED TO ADD IN MORE FUNCTIONS HERE


*/
//   int main() {
//         GetAll();
//   }

