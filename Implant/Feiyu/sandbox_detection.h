#ifndef SANDBOX_DETECTION_H
#define SANDBOX_DETECTION_H
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <algorithm>
#include <string>

bool vmCheck() {
	HRESULT hres;

	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		cout << "Failed to initialize COM library. Error code = 0x"
			<< hex << hres << endl;
		return 1;
	}

	hres = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL
	);


	if (FAILED(hres))
	{
		cout << "Failed to initialize security. Error code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return 1;
	}

	IWbemLocator* pLoc = NULL;

	hres = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)&pLoc);

	if (FAILED(hres))
	{
		cout << "Failed to create IWbemLocator object."
			<< " Err code = 0x"
			<< hex << hres << endl;
		CoUninitialize();
		return 1;
	}

	IWbemServices* pSvc = NULL;
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		&pSvc
	);

	if (FAILED(hres))
	{
		cout << "Could not connect. Error code = 0x"
			<< hex << hres << endl;
		pLoc->Release();
		CoUninitialize();
		return 1;
	}

	cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;


	hres = CoSetProxyBlanket(
		pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
	);

	if (FAILED(hres))
	{
		cout << "Could not set proxy blanket. Error code = 0x"
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;
	}

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_ComputerSystem"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		cout << "Query for operating system name failed."
			<< " Error code = 0x"
			<< hex << hres << endl;
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return 1;
	}

	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;


	HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1,
		&pclsObj, &uReturn);

	VARIANT vtProp;
	HRESULT hr1 = pclsObj->Get(L"Manufacturer", 0, &vtProp, 0, 0);
	std::wstring manufacturer(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
	hr1 = pclsObj->Get(L"Model", 0, &vtProp, 0, 0);
	std::wstring model(vtProp.bstrVal, SysStringLen(vtProp.bstrVal));
	std::transform(model.begin(), model.end(), model.begin(), ::toupper);
	if (manufacturer == L"microsoft corporation" && model.find(L"VIRTUAL") != std::string::npos
		|| manufacturer.find(L"vmware") != std::string::npos
		|| model == L"VIRTUALBOX") {
		wcout << "Manufacturer : " << manufacturer << "\nModel : " << model << endl;
		VariantClear(&vtProp);
		return true;
	}
	wcout << "Manufacturer : " << manufacturer << "\nModel : " << model << endl;
	VariantClear(&vtProp);
	pclsObj->Release();
	return false;

	pSvc->Release();
	pLoc->Release();
	pEnumerator->Release();
	CoUninitialize();

	return 0;
}

bool vmDriverCheck() {
	struct stat buffer;
	return !(stat("%SystemRoot%\\System32\\drivers\\hgfs.sys", &buffer) == 0 || stat("%SystemRoot%\\System32\\drivers\\vmhgfs.sys", &buffer)
		|| stat("%SystemRoot%\\System32\\drivers\\prleth.sys", &buffer) || stat("%SystemRoot%\\System32\\drivers\\prlfs.sys", &buffer) || stat("%SystemRoot%\\System32\\drivers\\prlmouse.sys", &buffer) || stat("%SystemRoot%\\System32\\drivers\\prlvideo.sys", &buffer) || stat("%SystemRoot%\\System32\\drivers\\vmx86.sys", &buffer));
}

bool sandboxTimeCheck() {
	int currentTime = GetTickCount64();
	Sleep(6000);
	int sleepTimer = GetTickCount64() - 6000 - currentTime;
	if (sleepTimer >= 100) {
		return true;
	}
	return false;
}
