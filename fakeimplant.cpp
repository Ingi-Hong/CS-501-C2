#include <windows.h>
#include <winhttp.h>
#include <string>
#include <iostream>


std::string makeHttpRequest(std::string fqdn, int port, std::string uri, bool useTLS) {
    char postdata[11] = "{\"id\":1}";
    printf(postdata);
    LPCWSTR additionalHeaders = L"Content-Type: application/json\r\n";
    // Used Windows documentation for every function

    // Flag of the Server : 0 HTTP / 1 HTTPS
    DWORD dwFlags;

    // Attempted to use this, but resorted to INTERNET_DEFAULT_PORT since INTERNET_DEFAULT_HTTP_PORT wasn't working
    if (useTLS == 0) {
        dwFlags = WINHTTP_FLAG_ESCAPE_DISABLE;
    }
    else {
        dwFlags = WINHTTP_FLAG_SECURE;
    }



    // Initializes variables for the Handles

    /* Converts from string to wstring to LPCWSTR
       https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
    */
    std::string result;
    std::wstring fqdn_wstring(fqdn.begin(), fqdn.end());
    LPCWSTR fqdnptr = fqdn_wstring.c_str();

    // The WinHttpOpen function initializes, for an application, the use of WinHTTP functions and returns a WinHTTP-session handle.
    HINTERNET hSession = WinHttpOpen(fqdnptr, WINHTTP_ACCESS_TYPE_NO_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession) {
        return "WinHttpOpen Failed";
    }

    // The WinHttpConnect function specifies the initial target server of an HTTP request and returns an HINTERNET connection handle to an HTTP session for that initial target.
    HINTERNET hConnect = WinHttpConnect(hSession, fqdnptr, port, 0);

    if (!hConnect) {
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

    if (!hRequest) {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        return "WinHttpOpenRequest Failed";
    }

    // The WinHttpSendRequest function sends the specified request to the HTTP server.
    if (!WinHttpSendRequest(hRequest, additionalHeaders, -1, postdata, 11, 11, 0)) {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpSendRequest Failed";
    }

    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        WinHttpCloseHandle(hSession);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hRequest);
        return "WinHttpReceiveResponse Failed";
    }


    DWORD lpdwNumberOfBytesAvailable = 0;
    LPSTR lpBuffer = NULL;

    do {

        lpdwNumberOfBytesAvailable = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &lpdwNumberOfBytesAvailable)) {
            WinHttpCloseHandle(hSession);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hRequest);
            return "WinHttpQueryDataAvailable Failed";
        }

        // Initializing Buffer with 4096 bytes
        lpBuffer = new char[4096];
        ZeroMemory(lpBuffer, 4096);

        DWORD lpdwNumberOfBytesRead = 0;

        if (!WinHttpReadData(hRequest, lpBuffer, lpdwNumberOfBytesAvailable, &lpdwNumberOfBytesRead)) {
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
    //result = fqdn + uri;
    return result;

}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cout << "Incorrect number of arguments: you need 4 positional arguments" << std::endl;
        return 0;
    }

    std::string fqdn = std::string(argv[1]);
    int port = std::stoi(argv[2]);

    std::string uri = std::string(argv[3]);
    int  useTLS = std::stoi(argv[4]);
    bool tls;

    if (useTLS == 1) {
        tls = true;
    }
    else if (useTLS == 0) {
        tls = false;

    }
    else {
        std::cout << "bad value for useTls" << std::endl;
        return 0;
    }
    std::cout << makeHttpRequest(fqdn, port, uri, tls) << std::endl;
    return 0;

}