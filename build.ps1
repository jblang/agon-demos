$filename = $args[0]
.\ez80asm.exe -l "$filename.asm"
python3 .\send.py "$filename.bin" COM6