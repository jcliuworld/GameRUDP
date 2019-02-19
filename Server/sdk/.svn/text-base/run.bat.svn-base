@echo off
set dir=
set "dir=%\work\age\abase\"
set/a n=0
for /f "delims=" %%a in ('dir /b /s /a-d %dir% *.cpp *.h') do (
    for /f %%b in ('type "%%a"') do set/a n+=1
    call echo.%%a  ---%%n%%----
)>%temp%\list.txt
move %temp%\list.txt %dir%list.txt 