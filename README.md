# manymouse_osc

Pure C implementation of [manymouse](https://github.com/icculus/manymouse) OSC sender. <br>
Intended to get native delta values of mice (mouse) and trackballs, and use them from Unity or etc. 

## Usage

> [!NOTE]
> There are pre-built binaries in [Releases](https://github.com/anno-git/manymouse_osc/releases/).

```txt
manymouse_osc

options:
  -h, --help               show this help message and exit
  -p, --port               OSC out port (default: 9000)
  -t, --host               OSC out host (default: 127.0.0.1)
```

### OSC

- /manymouse/relmotion 3 1 -1 (type tags: iii)
    - device index, axis (0/horizontal or 1/vertical), delta

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel 8 -j 8 --config Release
```

## License (Dependencies)

- [tinyosc](https://github.com/funatsufumiya/tinyosc): ISC License / Copyright (c) 2015, Martin Roth <mhroth@gmail.com>
- [manymouse](https://github.com/icculus/manymouse): Zlib License / Copyright (c) 2005-2022 Ryan C. Gordon <icculus@icculus.org>.
- [argparse](https://github.com/pollyren/argparse): MIT License / Copyright (c) 2025 pollyren