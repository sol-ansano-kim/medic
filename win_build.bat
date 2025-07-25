@echo off

if not exist ".build" (
    mkdir .build
)

if not exist ".build\2025" (
    mkdir .build\2025
)
cmake -S . -B .build\2025 -DCMAKE_INSTALL_PREFIX=.\release -DMAYA_VERSION=2025 -G "Visual Studio 17 2022"
cmake --build .build\2025 --config Release --target install -j9


if not exist ".build\2026" (
    mkdir .build\2026
)
cmake -S . -B .build\2026 -DCMAKE_INSTALL_PREFIX=.\release -DMAYA_VERSION=2026 -G "Visual Studio 17 2022"
cmake --build .build\2026 --config Release --target install -j9
