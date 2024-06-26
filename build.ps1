$filename = $args[0]
.\ez80asm.exe "$filename.asm"
python3 .\send.py "$filename.bin" COM6