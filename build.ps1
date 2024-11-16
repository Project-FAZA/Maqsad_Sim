# build.ps1

gcc -m32 -I include -L lib -o main (Get-ChildItem -Recurse -Filter *.c -Path src | ForEach-Object { $_.FullName }) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
