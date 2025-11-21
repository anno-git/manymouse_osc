# manymouse_osc

Pure C implementation of manymouse osc sender.

- Original code of manymouse: https://github.com/icculus/manymouse
- Original code of tinyosc: https://github.com/funatsufumiya/tinyosc (forked ver)

## Original README

[README_orig.txt](./README_orig.txt)

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 8 -j 8 --config Release
```