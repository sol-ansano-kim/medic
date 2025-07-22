rm -rf .build/2025
mkdir -p .build/2025
cmake -S . -B .build/2025 -DMEDIC_TESTER_PATH=custom/tester -DCMAKE_INSTALL_PREFIX=./release -DMAYA_VERSION=2025
cmake --build .build/2025  --config Release --target install -j9

rm -rf .build/2026
mkdir -p .build/2026
cmake -S . -B .build/2026 -DCMAKE_VERBOSE_MAKEFILE=1 -DMEDIC_TESTER_PATH=custom/tester -DCMAKE_INSTALL_PREFIX=./release -DMAYA_VERSION=2026
cmake --build .build/2026  --config Release --target install -j9
