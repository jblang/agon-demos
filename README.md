# Z80 Plasma Demo

This is a nice, flexible plasma effect for Z80-based computers.  It is ported from the C64 demo [Plascii Petsma](https://csdb.dk/release/?id=159933) by Camelot using the custom gradient characters from [Produkthandler Kom Her](https://csdb.dk/release/?id=760), also by Camelot. 

## Features 

- change the palette independent of the effect
- hold a particular effect on screen indefinitely
- switch immediately to a new effect
- runtime generation of random effects
- adjust parameters to customize an effect

## Further Information

- [Youtube Video](https://www.youtube.com/watch?v=l6ClY4K2lYk) (Agon version)
- [Youtube Video](https://www.youtube.com/watch?v=aIT_a1AZj5A) (TMS9918 version)
- [Hackaday.io article](https://hackaday.io/project/159057-game-boards-for-rc2014/log/183324-plasma-effect-for-tms9918) 

## Supported Platforms

- [Agon](https://www.thebyteattic.com/p/agon.html)-compatible system

### Original Version

For now, use [this version](https://github.com/jblang/TMS9918A/blob/master/examples/plasma.asm) on these platforms:

- [RC2014](https://rc2014.co.uk/) or [RCBus](https://smallcomputercentral.com/rcbus/)-compatible system with [TMS9918A video board](https://github.com/jblang/TMS9918A/)
- [MSX](https://www.msx.org/forum/msx-talk/development/new-plasma-effect-for-tms9918)

I plan to make a common code base in this repo to support both Agon and the RCBus and MSX platforms. I have the code for the other platforms here (tms.inc, z180.inc, rcmsx.inc) but it has not been tested and certainly won't build or work without some fixes. 

## Recommended Assemblers

- Agon (eZ80): [agon-ez80asm](https://github.com/envenomator/agon-ez80asm)
- Generic Z80/Z180: [sjasm](http://www.xl2s.tk/) or [sjasmplus](https://github.com/z00m128/sjasmplus)

## MIT License

Copyright 2018-2024 J.B. Langston

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE.