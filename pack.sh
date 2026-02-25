#!/bin/sh

rm -f libbanio.zip libbanio.nrm libbanio.so libbanio.dll lib/build/libbanio.so lib/build/libbanio.so.dll >/dev/null 2>&1

make clean
make -j$(nproc) && \
~/git/N64Recomp/build/RecompModTool mod.toml build/
mv build/libbanio.nrm ./

pushd lib
  make clean
  make -j$(nproc)
  mv build/libbanio.so ../
  mv build/libbanio.dll ../
popd

zip -q9 libbanio.zip libbanio.nrm libbanio.so libbanio.dll manifest.json icon.png README.md
