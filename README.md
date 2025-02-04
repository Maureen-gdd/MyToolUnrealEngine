# MyTool 🚀

A tool to help with Unreal Engine project management. This tool allows you to interact with your Unreal project files by showing important project information, building the project, and packaging it for deployment.

---

## How to use 🛠️

1. **Build the project**  
   First, you need to build the project (.sln file) to generate the `.exe` file. The output `.exe` will be located at the following path:  
   `./MyToolTP1/x64/Debug/` ou `./MyToolTP1/x64/Release/`
   
2. **Copy the `.exe`**  
   Once built, copy the `.exe` to the root of your **From Source Unreal Engine** setup. This is the root folder where your Unreal Engine source code is located.

3. **Open CMD**  
   Open a **Command Prompt** or **PowerShell** window, and use one of the following commands based on your needs:

--- 

⚠️**Absolute Paths**: A key reminder to always use absolute paths in all commands to avoid errors.

# Shows name, version and used plugins of the project
MyTool.exe [PATH_of_your_Uproject_file] show-infos

# Builds project
MyTool.exe [PATH_of_your_Uproject_file] build

# Package project
MyTool.exe [PATH_of_your_Uproject_file] package [PATH_where_you_want_your_package_to_be]