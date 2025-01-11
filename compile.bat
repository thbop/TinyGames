@echo off
cl /c /O1 /GS- /DNDEBUG /Fo%1.obj %1.c
crinkler /OUT:%1.exe %1.obj kernel32.lib user32.lib
rm %1.obj