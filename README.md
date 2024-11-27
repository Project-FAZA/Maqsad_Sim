# Oye Suno

iss ko compile karnay ke liay ye command run karo

```powershell
./build.ps1
```

Agar powershell masti kar raha hai toh yeh DIRECTLY run karo

```powershell
gcc -m32 -I include -L lib -o main (Get-ChildItem -Recurse -Filter *.c -Path src | ForEach-Object { $_.FullName }) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
```

Aur phir

```powershell
./main
```

LEKIN AGAR TUMHAY SIRF GAME KHELNA HAI (parhai warhai toh hoti nahi tum kameenou se) then download these files/folders only

```
fonts/
SoundEffects/
images/
files/
*.dll
main.c
```

**OHKAY** Dats AAL. 👍
