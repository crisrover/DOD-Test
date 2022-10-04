# DOD-Test

A very short and simple test covering cache line utilization and data organization comparing OOP approach vs DOD approach.

# Disclaimer

No error checking is performed, if it crashes it is because it runs oout of memory.
Closing big memory consumers like Chrome etc, will help.

## Results on personal machines:

### Intel(R) Core(TM) i3-4160 CPU @ 3.60GHz   3.60 GHz 8 GB RAM
Timed OOP        : 0.091859 s
Timed OOP Derived: 0.182082 s
Timed DOD        : 0.005636 s

### Intel(R) Core(TM) i3-2120 CPU @ 3.30GHz   3.30 GHz 8 GB RAM
Timed OOP        : 0.065113 s
Timed OOP Derived: 0.141823 s
Timed DOD        : 0.007353 s

