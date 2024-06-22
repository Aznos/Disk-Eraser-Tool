# Disk-Eraser-Tool

The simple safe and secure disk eraser tool for OSX.

## Features
- Securely erase your disk, making it impossible to recover data
- Erase multiple disks at once, saving time
- Simple and easy to use
<br>
<br>

# Installation
<h3>Normal Installation</h3>
<ol>
  <li>Download the latest release from the [Releases page](https://github.com/Aznos/Disk-Eraser-Tool/releases)</li>
  <li>Make sure the disk(s) you want to erase are unmounted from the system, this can be done with `diskutil unmountDisk /dev/diskX`</li>
  <li>Run `sudo det.exe` in the terminal</li>
  <li>Confirm the disk you'd like to erase, and begone with your data!</li>
</ol>
<h3>Compile from source</h3>
<ol>
    <li>Clone the repo with `git clone https://github.com/Aznos/Disk-Eraser-Tool/</li>
    <li>Run `make` in the root directory of the project</li>
    <li>Optionally run `make run` to automatically unmount the disk(s) and run the program</li>
    <li>Otherwise, unmount the disk(s) and run `sudo bin/det`</li>
</ol>

# Usage
- Run the program with `sudo det.exe`
- Select the disk(s) you'd like to erase
- Confirm the disk(s) you'd like to erase
- Wait for the program to finish (this may take a while depending on the size of the disk)

# License
This project is licensed under the MIT License - see the [LICENSE](https://github.com/Aznos/Disk-Eraser-Tool/blob/main/LICENSE)

# Disclaimer
This project was made as a hobby project, and this is not intended for professional use. This project was made for educational purposes only, and I am not responsible for any data loss or damage caused by this program. Use at your own risk.