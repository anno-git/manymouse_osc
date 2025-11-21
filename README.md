# manymouse_osc

Pure C implementation of manymouse osc sender. <br>
Intended to get native delta values of mice (mouse) and trackballs, and use them from Unity or etc. 

- Original code of manymouse: https://github.com/icculus/manymouse
- Original code of tinyosc: https://github.com/funatsufumiya/tinyosc (forked ver)

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 8 -j 8 --config Release
```

## License

- tinyosc: ISC License / Copyright (c) 2015, Martin Roth <mhroth@gmail.com>
- manymouse: Zlib License / Copyright (c) 2005-2022 Ryan C. Gordon <icculus@icculus.org>.