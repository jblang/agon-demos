# Agon Demos

This is intended to become a collection of assembly-language demos for the [Agon](https://www.thebyteattic.com/p/agon.html) family of computers:

- [AgonLight](https://github.com/TheByteAttic/AgonLight)
- [AgonLight2](https://www.olimex.com/Products/Retro-Computers/AgonLight2/open-source-hardware)
- [Agon Console8](https://heber.co.uk/agon-cosole8/)

Right now there's only one demo (Plasma), but once I get that fully optimized, I plan to port over [Nyan Cat](https://www.youtube.com/watch?v=VAgBXoOtOkI), and after that... who knows?

Perhaps most importantly, these demos are intended as a vehicle for developing a [library](vdu) of reusable assembly language routines for interacting with the [VDP](https://github.com/agonconsole8/agon-vdp).

## Resources

- The [Agon Community Docs](https://agonconsole8.github.io/agon-docs/) will be of great help in making sense of any of this.
- [agon-ez80asm](https://github.com/envenomator/agon-ez80asm) is what I use to build this. Available both as a native Agon assembler, and as cross-assemblers for modern platforms.
- YouTube Demos:
    - [Agon Version](https://www.youtube.com/watch?v=l6ClY4K2lYk)
    - [TMS9918A Version](https://www.youtube.com/watch?v=aIT_a1AZj5A)
- [Hackaday.io Article](https://hackaday.io/project/159057-game-boards-for-rc2014/log/183324-plasma-effect-for-tms9918) explaining the original code for the TMS9918A

## Plasma Demo

A beautiful and flexible plasma effect with the following features:

- easily define effects in a standard and concise format
- change the palette independent of the effect
- hold a particular effect on screen indefinitely
- switch immediately to a new effect
- runtime generation of random effects
- adjust parameters to customize effect

### Keyboard Commands

- `q`: quit
- `h`: hold current effect on/off
- `p`: switch palette
- `n`: next effect
- `a`: animation on/off
- `r`: toggle random/playlist

The original version had additional commands that are yet to be implemented in the Agon version.

### Other Versions

- My Z80 port was derived from the C64 demos [Plascii Petsma](https://csdb.dk/release/?id=159933) and [Produkthandler Kom Her](https://csdb.dk/release/?id=760), both by Camelot.
- I wrote the [Z80/TMS9918A port](https://github.com/jblang/TMS9918A/blob/master/examples/plasma.asm) for the [RCBus](https://smallcomputercentral.com/rcbus/)-compatible [TMS9918A board](https://github.com/jblang/TMS9918A) I designed.
- The TMS9918A port can also be configured at build-time to [run on an MSX](https://www.msx.org/forum/msx-talk/development/new-plasma-effect-for-tms9918), or at least on the [BlueMSX](http://bluemsx.msxblue.com/) emulator I tested it with.


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