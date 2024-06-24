# Disk-Eraser-Tool

A simple, safe, and secure disk eraser tool for macOS.

## Features
- Securely erase your disk, making it impossible to recover data
- Erase multiple disks at once, saving time
- Simple and easy to use

## Installation

### Normal Installation
1. Download the latest release from the [Releases page](https://github.com/Aznos/Disk-Eraser-Tool/releases).
2. Make sure the disk(s) you want to erase are unmounted from the system. This can be done with:
   ```diskutil unmountDisk /dev/diskX```
3. Run the program (CLI):
   ```sudo ./det.exe```
   Run the program (GUI):
   ```sudo ./det.exe gui```
4. Confirm the disk you'd like to erase and proceed with erasing your data!

### Compile from Source
1. Clone the repository:
   ```git clone https://github.com/Aznos/Disk-Eraser-Tool/```
2. Navigate to the root directory of the project:
   ```cd Disk-Eraser-Tool```
3. Compile the project:
   ```make```
4. Optionally, you can:
   Edit the disk name in the Makefile to automatically unmount the disk (CLI)
   ```make run```
   GUI:
   ```make rungui
   This will automatically unmount the disk(s) and run the program.
5. Otherwise, unmount the disk(s) manually and run (CLI):
   ```sudo bin/det```
   Run the program (GUI):
   ```sudo bin/det gui```

## Usage
1. Run the program (CLI):
   ```sudo ./det.exe```
   Run the program (GUI):
   ```sudo ./det.exe gui```
2. Select the disk(s) you'd like to erase.
3. Confirm your selection.
4. Wait for the program to finish (this may take a while depending on the size of the disk).

## License
This project is licensed under the MIT License - see the [LICENSE](https://github.com/Aznos/Disk-Eraser-Tool/blob/main/LICENSE) file for details.

## Disclaimer
This project was made as a hobby project and is not intended for professional use. It was created for educational purposes only, and I am not responsible for any data loss or damage caused by this program. Use at your own risk.