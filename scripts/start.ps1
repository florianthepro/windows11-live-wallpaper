# ===== start.ps1 =====
$ErrorActionPreference = "Stop"

$base = "$env:ProgramData\ClassicLiveWallpaper"
$exe  = "$base\livewallpaper.exe"

New-Item -ItemType Directory -Force -Path $base | Out-Null

irm https://raw.githubusercontent.com/DEINNAME/classic-live-wallpaper/main/build/livewallpaper.exe `
  -OutFile $exe

Start-Process $exe "https://intranet.local" -WindowStyle Hidden
