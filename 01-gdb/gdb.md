> gdb program // Start debugging program
> gdb -e program -c core // Start with core dump
> gdb program --pid pid // Start attached to a process id.
(gdb) break main // Set breakpoint in function main
(gdb) break 12 // Set breakpoint on line 12 in current file
(gdb) break file.c:12 // Set breakpoint on line 12 in file file.c
(gdb) run // Run the program and debug it.
(gdb) run arg1 arg2 ... // Start debug and pass arg1 arg2 to program
(gdb) layout next // Change to next layout src, reg, ...
(gdb) Ctrl-X A // Switch into Layout mode on and off
(gdb) next // Step Over
(gdb) step // Step In
(gdb) finish // Step Out
(gdb) continue // Continue to next breakpoint
(gdb) bt // Backtrace
(gdb) frame 1 // Show frame 1 in the backtrace
(gdb) print variable // Print value of variable
(gdb) print *ptr // Print value of a pointer
(gdb) info locals // Show locals
(gdb) F8 // Previous command
(gdb) F6 
