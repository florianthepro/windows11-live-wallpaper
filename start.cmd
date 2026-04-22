@echo off
set TARGET=C:\LiveWallpaper
if not exist %TARGET% mkdir %TARGET%

powershell -Command ^
"Invoke-WebRequest https://raw.githubusercontent.com/florianthepro/windows11-live-wallpaper/main/releases/livewallpaper.exe -OutFile %TARGET%\livewallpaper.exe"

start "" "%TARGET%\livewallpaper.exe" https://tanum-launcher.tcsoc.net/dashboard/
