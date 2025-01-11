@echo off
cl /c /O1 /GS- /DNDEBUG /Foa.obj a.c
crinkler /OUT:a.exe a.obj kernel32.lib user32.lib
rm a.obj