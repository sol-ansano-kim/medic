# 2025
if [ ! -d ".build/2025" ]; then
  mkdir -p .build/2025
fi
cmake -S . -B .build/2025 -DCMAKE_INSTALL_PREFIX=./release -DMAYA_VERSION=2025
cmake --build .build/2025 --config Release --target install -j8

# 2026
if [ ! -d ".build/2026" ]; then
  mkdir -p .build/2026
fi
cmake -S . -B .build/2026 -DCMAKE_INSTALL_PREFIX=./release -DMAYA_VERSION=2026
cmake --build .build/2026 --config Release --target install -j8
