@echo off
@echo # 'UserInitMprLogonScript'
@if exist c:\test\UserInitMprLogonScript.log @del c:\test\UserInitMprLogonScript.log
@echo UserInitMprLogonScript executed !> c:\test\UserInitMprLogonScript.log
@start "c:\Windows\System32\notepad.exe"
@pause