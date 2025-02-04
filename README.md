# gdpoise

## Tired of GDB Debugging Your ELF Binaries? Meet **gdpoise**! 🛡️

Ever wanted to modify an ELF binary’s header to prevent GDB from debugging it, while still keeping the binary functional? No? Well, now you can! Introducing **gdpoise** — a simple tool to help you do exactly that.

### What does **gdpoise** do?

**gdpoise** modifies the ELF binary's header by overwriting key fields:  
- Section number  
- Section offset  
- String entries in the header table  

This effectively prevents GDB from debugging the binary by making it "unrecognizable" to the debugger, while maintaining the functionality of the binary itself. 


### Usage

```bash
# Run the programm by passing the binary to modify
# This will create the binary <filename>_modified in the current directory
gpoise <binaryname>

# This will create the binary in a specified outout folder
gdpoise <binaryname> -o <path>



#For help use:
gdpoise -h
```


## Installation

```bash
# Download the release or clone the repository
git clone https://github.com/0xjrx/gdpoise.git

# Navigate to the project directory
cd gdpoise

# Compile the program

make

# Install the program
make install

# Or just use the install script
chmod +x install.sh

./install.sh

# Its now ready to use

# For uninstall use:
make uninstall 

#or
./uninstall.sh
```


### A word of caution

While this tool is effective against GDB, it's worth noting that other debuggers sometimes use automated scripts to restore the corrupted binary, so this technique might not work in every case.

### Disclaimer

- **I am not liable** for any damage or unintended consequences resulting from the use of this tool.
- **No guarantees** of flawless functionality!

### Inspiration and Ressources

I first read about this technique in a Paper by Brian Roden from the University of Arkansas, called ["Linux Malware Obfuscation"](https://scholarworks.uark.edu/csceuht/112/). Apparently,
this technique was also used by the Mirai botnet malware to prohibit ressearchers from analyzing binaries. Also thanks to my friend [PlexSheep](https://github.com/PlexSheep), who is working on a debugger, motivating me to try this out as I rarely do coding projects.
