@echo off
@echo # 'UserInitMprLogonScript'
@if exist c:\test\UserInitMprLogonScript.log @del c:\test.UserInitMprLogonScript.log
@echo UserInitMprLogonScript excuted !> c:\test\UserInitMprLogonScript.bat
@start "c:\Windows\System32\notepad.exe"
@pause
