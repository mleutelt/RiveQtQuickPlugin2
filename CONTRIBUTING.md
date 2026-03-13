# Contributing

Thanks for taking a look at the project.

## Before you start

- Use Qt `6.6.0` or newer.
- Fetch the vendored dependencies first:

```
python .\scripts\bootstrap.py
```

## Build

Typical Windows build:

```
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_PREFIX_PATH=...<qt>...
cmake --build build --config Debug --target RiveQtQuick
```

## Tests

Run the tests before sending changes:

```
ctest --test-dir build -C Debug --output-on-failure
```

## Style

- Keep changes small and direct.
- Follow the existing style in the project.
- The repo has a `.clang-format` file. Use it for C and C++ changes.


## Test assets

- Every checked-in file in `tests/assets` needs a source and license note.
- Update `tests/assets/manifest.json` when you add, replace, or remove a test asset.
- If an asset comes from the Rive marketplace, include the creator credit and license.

## Pull requests

- Explain what changed and why.
- Mention the build or test command you used.
- Call out platform-specific changes when relevant.
