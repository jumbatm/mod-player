# MOD Music Player (Work in progress)
If you've heard of the demoscene, especially in the era of the Amiga and the ZX Spectrum, you've likely heard tracker files, and the [MOD file format](https://en.wikipedia.org/wiki/MOD_(file_format)). These files store music in a way similar to MIDI nowadays, in the way that data for what *note* to play is stored rather than the sound of the instrument playing that note. 

Unlike MIDI, the sound of each instrument a single note is stored as well, which is then frequency-shifted higher or lower to sound like the instrument is playing different notes by playing it at specific sampling rates higher or lower than the one it was recorded at.

**Please note that this MOD Player is still in progress and does not work yet.** If you're looking for a program to play tracker music you have, don't use this one just yet. Instead, I recommend using the open-source and much loved [VLC player](http://www.videolan.org/vlc/). In addition to MOD files, it will also play all other tracker music file formats that I've seen.

## Dependencies
- PulseAudio
