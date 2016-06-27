MTD Bid Calculation:
=======================

### Requirements

To build from source, ensure that you meet the following requirements:

1. C++11 compiler installed (gcc/clang)
2. Make is installed (Else, manual compilation and execution required. Read the `Makefile` for more details)

*Note: Limited Windows support, if any. Untested. Linux platform recommended. All commands mentioned in this readme are targeted for bash*

### Build

1. Compile from source:
  ```
  $ make all
  ```

2. Run (eg: Assuming 15 VMs):
  ```
  $ make run vmcount=15
  ```

### Cleanup

To remove compiled executable:
```
$ make clean
```
