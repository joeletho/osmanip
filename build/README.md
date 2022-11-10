# Using CMake

For information regarding CMake file hierarchy and best practices, refer
to [this StackOverflow post](https://stackoverflow.com/questions/57412591/cmake-project-build-only-one-specific-executable-and-its-dependencies).

#### Example:

```cmake
    .
    |-- build    <------------ Run CMake commands from here.
    |-- bin
    |-- lib1
    |   |-- CMakeLists.txt
    |-- lib2
    |   |-- CMakeLists.txt
    |-- CMakeLists.txt
```
---
### Instructions

From ```osmanip/build/```,

- To configure (generate CMakeCache.txt), use

```
cmake ..
```

- Set option when configuring cmake by using the command

```
cmake .. -D OSM_EXAMPLES=ON
```

- After configuring the target, build it by using

```
cmake --build .               // Note the current "." directory, not the parent ".."
```

- Build specific target with

```
cmake --build . --target <target-name>
```

- To "clean" and/or rebuild the cache, use

```
cmake --fresh ..              // Alternatively, delete the directory contents and reconfigure (Keep this file)
```
---
### Additional notes

- ```OSM_EXAMPLES``` is left as an option to give the user the ability to use the library without generating the binaries unnecessarily. ```OSM_EXAMPLES``` can be enabled by using the ```-D``` option when configuring CMake. See example above. Furthermore, the option can be enabled automatically by setting the option to ```ON``` in the top ```CMakeLists.txt``` file.
