# tetris
My ascii/cli rendition of tetris. From Wikipedia:

> The objective of [tetris] is to manipulate tetriminos, by moving each one sideways and/or rotating by quarter-turns, so that they form a solid horizontal line without gaps

<img src=docs/out.gif width=500px>

## Installation

### Git:
If on linux, install libncurses-dev as dependency.
```bash
git clone 'https://github.com/k-vernooy/tetris' && cd tetris
make  # if on linux, run 'make ubuntucompile' instead for proper characters
sudo make install
```
### Homebrew:
```bash
brew tap k-vernooy/tap
brew install tetris
```

## Usage 
### Control:
Use the arrow keys to control the falling blocks:  
- <kbd>↑</kbd> to rotate the falling block
- <kbd>→</kbd> to move right
- <kbd>←</kbd> to move left
- <kbd>↓</kbd> to soft drop
- <kbd>SPACE</kbd> to hard drop
- <kbd>CTRL</kbd>+<kbd>C</kbd> at any time to quit the game
