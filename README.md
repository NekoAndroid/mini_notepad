# mini_notepad
Tiny, no fuss CLI notepad written in C

## What is this?
It's a lightweight notepad app designed to run on very, very constrainted environments, using just above 66KiB of RAM, and about 18KiB of storage space (about 20KB afaik)
and to be as difficult to distract as possible

## Commands:
All commands follow the same pattern: </[COMMAND]>, with [COMMAND]
being either of these below: (the commands are lowercase sensitive btw)

save: Saves the file to the file name (or path) you wish (Doesn't save the command though, it's removed from the file)

load: Loads a file (WARNING: No alterations are saved if you used this command! Be careful with it!)

exit: Exits the program (self-explanatory)
