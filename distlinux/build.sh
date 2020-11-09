#!/bin/bash

cp ../endercalc staging/opt/endercalc
cp ../reference.pdf staging/opt/endercalc
rm staging/usr/bin/endercalc
cp ../README.md staging/usr/share/doc/endercalc/README
cp ../THIRD-PARTY.txt staging/usr/share/doc/endercalc/THIRD-PARTY

find staging -type d -exec chmod 0755 {} \;
find staging -type f -exec chmod 0644 {} \;
chmod +x staging/opt/endercalc/endercalc
ln -s /opt/endercalc/endercalc staging/usr/bin/endercalc

dpkg-deb --root-owner-group --build staging endercalc_1.1_amd64.deb
lintian endercalc_1.1_amd64.deb --info --suppress-tags dir-or-file-in-opt
