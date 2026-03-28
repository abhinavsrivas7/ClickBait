# Run locally on windoows
- install mysys2 and mingw64
- rename file: `./msys64/ucrt64/bin/%make%.exe` to `./msys64/ucrt64/bin/%make%.exe` 
- make run

# Debug configuration for VS Code

## launch.json (sample)
```json
{
    "version": "0.2.0",
    "configurations": [{
        "name": "(gdb) Launch",
        "type": "cppdbg",
        "request": "launch",
        "program": "${workspaceFolder}/bin/click-bait.exe",
        "args": [],
        "stopAtEntry": false,
        "cwd": "${workspaceFolder}",
        "environment": [],
        "externalConsole": true,
        "MIMode": "gdb",
        "miDebuggerPath": "C:\\msys64\\ucrt64\\bin\\gdb.exe",
        "setupCommands": [{
            "description": "Enable pretty-printing for gdb",
            "text": "-enable-pretty-printing",
            "ignoreFailures": true
        }],
        "preLaunchTask": "Build with Makefile"   
    }]
}
```


## tasks.json (sample)
```json
{
    "version": "2.0.0",
    "tasks": [{
        "label": "Build with Makefile",
        "type": "shell",
        "command": "make", 
        "args": [ "-B", "-j" ],
        "options": { "cwd": "${workspaceFolder}" },
        "group": { "kind": "build", "isDefault": true },
        "problemMatcher": ["$gcc"]   
    }]
}
```