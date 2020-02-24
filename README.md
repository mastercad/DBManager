# MYSQL installieren / bauen:
sudo apt install libmysqlclient-dev libqt5sql5-mysql
sudo apt install libmysqlcppconn-dev libclang-cpp10 libllvm10

# Beim start von DBManager kam immer Speicherzugriffsfehler
- Das Problem trat immer nur auf, wenn ich es geuploadet habe. scheinbar schneidet hier der FTP etwas ab

# Wenn ich das Programm nun auf den USB Stick packe und es starte kommt:
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Sql.so.5: version `Qt_5' not found (required by ./DBManager)
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5: version `Qt_5' not found (required by ./DBManager)
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5: version `Qt_5.9' not found (required by ./DBManager)
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5: version `Qt_5' not found (required by ./DBManager)
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5: version `Qt_5' not found (required by ./DBManager)

# mit ldd ./DBManager auf dem anderen Rechner kommt:
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Sql.so.5: version `Qt_5' not found (required by ./DBManager)
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5: version `Qt_5' not found (required by ./DBManager)
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5: version `Qt_5.9' not found (required by ./DBManager)
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5: version `Qt_5' not found (required by ./DBManager)
./DBManager: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5: version `Qt_5' not found (required by ./DBManager)
        linux-vdso.so.1 =>  (0x00007ffe3577b000)
        libQt5Widgets.so.5 => /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5 (0x00007f0852a33000)
        libQt5Gui.so.5 => /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5 (0x00007f08524eb000)
        libQt5Sql.so.5 => /usr/lib/x86_64-linux-gnu/libQt5Sql.so.5 (0x00007f08534b8000)
        libQt5Core.so.5 => /usr/lib/x86_64-linux-gnu/libQt5Core.so.5 (0x00007f0852015000)
        libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f0851c93000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f0851a7d000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f08516b3000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f0851496000)
        libgobject-2.0.so.0 => /usr/lib/x86_64-linux-gnu/libgobject-2.0.so.0 (0x00007f0851243000)
        libglib-2.0.so.0 => /lib/x86_64-linux-gnu/libglib-2.0.so.0 (0x00007f0850f32000)
        libX11.so.6 => /usr/lib/x86_64-linux-gnu/libX11.so.6 (0x00007f0850bf8000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f08508ef000)
        libpng12.so.0 => /lib/x86_64-linux-gnu/libpng12.so.0 (0x00007f08506ca000)
        libharfbuzz.so.0 => /usr/lib/x86_64-linux-gnu/libharfbuzz.so.0 (0x00007f085046c000)
        libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f0850252000)
        libGL.so.1 => /usr/lib/nvidia-390/libGL.so.1 (0x00007f084ff15000)
        libicui18n.so.55 => /usr/lib/x86_64-linux-gnu/libicui18n.so.55 (0x00007f084fab3000)
        libicuuc.so.55 => /usr/lib/x86_64-linux-gnu/libicuuc.so.55 (0x00007f084f71f000)
        libpcre16.so.3 => /usr/lib/x86_64-linux-gnu/libpcre16.so.3 (0x00007f084f4b8000)
        libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f084f2b4000)
        librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f084f0ac000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f0853314000)
        libffi.so.6 => /usr/lib/x86_64-linux-gnu/libffi.so.6 (0x00007f084eea4000)
        libpcre.so.3 => /lib/x86_64-linux-gnu/libpcre.so.3 (0x00007f084ec33000)
        libxcb.so.1 => /usr/lib/x86_64-linux-gnu/libxcb.so.1 (0x00007f084ea11000)
        libfreetype.so.6 => /usr/lib/x86_64-linux-gnu/libfreetype.so.6 (0x00007f084e767000)
        libgraphite2.so.3 => /usr/lib/x86_64-linux-gnu/libgraphite2.so.3 (0x00007f084e541000)
        libnvidia-tls.so.390.87 => /usr/lib/nvidia-390/tls/libnvidia-tls.so.390.87 (0x00007f084e33d000)
        libnvidia-glcore.so.390.87 => /usr/lib/nvidia-390/libnvidia-glcore.so.390.87 (0x00007f084c535000)
        libXext.so.6 => /usr/lib/x86_64-linux-gnu/libXext.so.6 (0x00007f084c323000)
        libicudata.so.55 => /usr/lib/x86_64-linux-gnu/libicudata.so.55 (0x00007f084a86c000)
        libXau.so.6 => /usr/lib/x86_64-linux-gnu/libXau.so.6 (0x00007f084a668000)
        libXdmcp.so.6 => /usr/lib/x86_64-linux-gnu/libXdmcp.so.6 (0x00007f084a462000)

- auf dem Entwicklungsrechner:
linux-vdso.so.1 (0x00007ffc96d7f000)
        libQt5Widgets.so.5 => /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5 (0x00007f2560aa3000)
        libQt5Gui.so.5 => /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5 (0x00007f256033a000)
        libQt5Sql.so.5 => /usr/lib/x86_64-linux-gnu/libQt5Sql.so.5 (0x00007f25600f1000)
        libQt5Core.so.5 => /usr/lib/x86_64-linux-gnu/libQt5Core.so.5 (0x00007f255f9a6000)
        libstdc++.so.6 => /usr/lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f255f61d000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f255f405000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f255f014000)
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f255edf5000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f255ea57000)
        libGL.so.1 => /usr/lib/x86_64-linux-gnu/libGL.so.1 (0x00007f255e7d2000)
        libpng16.so.16 => /usr/lib/x86_64-linux-gnu/libpng16.so.16 (0x00007f255e5a0000)
        libharfbuzz.so.0 => /usr/lib/x86_64-linux-gnu/libharfbuzz.so.0 (0x00007f255e302000)
        libz.so.1 => /lib/x86_64-linux-gnu/libz.so.1 (0x00007f255e0e5000)
        libicui18n.so.60 => /usr/lib/x86_64-linux-gnu/libicui18n.so.60 (0x00007f255dc44000)
        libicuuc.so.60 => /usr/lib/x86_64-linux-gnu/libicuuc.so.60 (0x00007f255d88d000)
        libdouble-conversion.so.1 => /usr/lib/x86_64-linux-gnu/libdouble-conversion.so.1 (0x00007f255d67c000)
        libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f255d478000)
        libglib-2.0.so.0 => /usr/lib/x86_64-linux-gnu/libglib-2.0.so.0 (0x00007f255d161000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f256153e000)
        libGLdispatch.so.0 => /usr/lib/x86_64-linux-gnu/libGLdispatch.so.0 (0x00007f255ceab000)
        libGLX.so.0 => /usr/lib/x86_64-linux-gnu/libGLX.so.0 (0x00007f255cc7a000)
        libfreetype.so.6 => /usr/lib/x86_64-linux-gnu/libfreetype.so.6 (0x00007f255c9c6000)
        libgraphite2.so.3 => /usr/lib/x86_64-linux-gnu/libgraphite2.so.3 (0x00007f255c799000)
        libicudata.so.60 => /usr/lib/x86_64-linux-gnu/libicudata.so.60 (0x00007f255abf0000)
        libpcre.so.3 => /lib/x86_64-linux-gnu/libpcre.so.3 (0x00007f255a97e000)
        libX11.so.6 => /usr/lib/x86_64-linux-gnu/libX11.so.6 (0x00007f255a646000)
        libxcb.so.1 => /usr/lib/x86_64-linux-gnu/libxcb.so.1 (0x00007f255a41e000)
        libXau.so.6 => /usr/lib/x86_64-linux-gnu/libXau.so.6 (0x00007f255a21a000)
        libXdmcp.so.6 => /usr/lib/x86_64-linux-gnu/libXdmcp.so.6 (0x00007f255a014000)
        libbsd.so.0 => /lib/x86_64-linux-gnu/libbsd.so.0 (0x00007f2559dff000)
        librt.so.1 => /lib/x86_64-linux-gnu/librt.so.1 (0x00007f2559bf7000)

# mit objdump -p ./DBManager kommt auf dem anderen Rechner:
./DBManager:     Dateiformat elf64-x86-64

Programm-Header:
    PHDR off    0x0000000000000040 vaddr 0x0000000000000040 paddr 0x0000000000000040 align 2**3
         filesz 0x00000000000001f8 memsz 0x00000000000001f8 flags r--
  INTERP off    0x0000000000000238 vaddr 0x0000000000000238 paddr 0x0000000000000238 align 2**0
         filesz 0x000000000000001c memsz 0x000000000000001c flags r--
    LOAD off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**21
         filesz 0x000000000004fc7b memsz 0x000000000004fc7b flags r-x
    LOAD off    0x000000000004ff28 vaddr 0x000000000024ff28 paddr 0x000000000024ff28 align 2**21
         filesz 0x00000000000030f0 memsz 0x0000000000003178 flags rw-
 DYNAMIC off    0x00000000000522b0 vaddr 0x00000000002522b0 paddr 0x00000000002522b0 align 2**3
         filesz 0x0000000000000250 memsz 0x0000000000000250 flags rw-
    NOTE off    0x0000000000000254 vaddr 0x0000000000000254 paddr 0x0000000000000254 align 2**2
         filesz 0x0000000000000044 memsz 0x0000000000000044 flags r--
EH_FRAME off    0x000000000004a070 vaddr 0x000000000004a070 paddr 0x000000000004a070 align 2**2
         filesz 0x0000000000000a8c memsz 0x0000000000000a8c flags r--
   STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
   RELRO off    0x000000000004ff28 vaddr 0x000000000024ff28 paddr 0x000000000024ff28 align 2**0
         filesz 0x00000000000030d8 memsz 0x00000000000030d8 flags r--

Dynamischer Abschnitt:
  NEEDED               libQt5Widgets.so.5
  NEEDED               libQt5Gui.so.5
  NEEDED               libQt5Sql.so.5
  NEEDED               libQt5Core.so.5
  NEEDED               libstdc++.so.6
  NEEDED               libgcc_s.so.1
  NEEDED               libc.so.6
  INIT                 0x000000000000f8c8
  FINI                 0x0000000000036c04
  INIT_ARRAY           0x000000000024ff28
  INIT_ARRAYSZ         0x0000000000000030
  FINI_ARRAY           0x000000000024ff58
  FINI_ARRAYSZ         0x0000000000000008
  GNU_HASH             0x0000000000000298
  STRTAB               0x0000000000003058
  SYMTAB               0x00000000000002e0
  STRSZ                0x00000000000040c7
  SYMENT               0x0000000000000018
  DEBUG                0x0000000000000000
  PLTGOT               0x0000000000252500
  PLTRELSZ             0x0000000000001ef0
  PLTREL               0x0000000000000007
  JMPREL               0x000000000000d9d8
  RELA                 0x0000000000007630
  RELASZ               0x00000000000063a8
  RELAENT              0x0000000000000018
  FLAGS                0x0000000000000008
  FLAGS_1              0x0000000008000001
  VERNEED              0x00000000000074f0
  VERNEEDNUM           0x0000000000000007
  VERSYM               0x0000000000007120
  RELACOUNT            0x00000000000000e5

Versionsreferenzen:
  von libgcc_s.so.1 benötigt:
    0x0b792650 0x00 11 GCC_3.0
  von libc.so.6 benötigt:
    0x06969194 0x00 12 GLIBC_2.14
    0x09691a75 0x00 10 GLIBC_2.2.5
    0x0d696914 0x00 09 GLIBC_2.4
  von libstdc++.so.6 benötigt:
    0x0297f871 0x00 14 GLIBCXX_3.4.21
    0x08922974 0x00 13 GLIBCXX_3.4
    0x0bafd179 0x00 07 CXXABI_1.3.9
    0x056bafd3 0x00 06 CXXABI_1.3
  von libQt5Sql.so.5 benötigt:
    0x00058a25 0x00 05 Qt_5
  von libQt5Gui.so.5 benötigt:
    0x00058a25 0x00 04 Qt_5
  von libQt5Core.so.5 benötigt:
    0x058a2819 0x00 08 Qt_5.9
    0x00058a25 0x00 03 Qt_5
  von libQt5Widgets.so.5 benötigt:
    0x00058a25 0x00 02 Qt_5

- auf dem Entwicklungsrechner:
DBManager:     Dateiformat elf64-x86-64

Programm-Header:
    PHDR off    0x0000000000000040 vaddr 0x0000000000000040 paddr 0x0000000000000040 align 2**3
         filesz 0x00000000000001f8 memsz 0x00000000000001f8 flags r--
  INTERP off    0x0000000000000238 vaddr 0x0000000000000238 paddr 0x0000000000000238 align 2**0
         filesz 0x000000000000001c memsz 0x000000000000001c flags r--
    LOAD off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**21
         filesz 0x000000000004fc7b memsz 0x000000000004fc7b flags r-x
    LOAD off    0x000000000004ff28 vaddr 0x000000000024ff28 paddr 0x000000000024ff28 align 2**21
         filesz 0x00000000000030f0 memsz 0x0000000000003178 flags rw-
 DYNAMIC off    0x00000000000522b0 vaddr 0x00000000002522b0 paddr 0x00000000002522b0 align 2**3
         filesz 0x0000000000000250 memsz 0x0000000000000250 flags rw-
    NOTE off    0x0000000000000254 vaddr 0x0000000000000254 paddr 0x0000000000000254 align 2**2
         filesz 0x0000000000000044 memsz 0x0000000000000044 flags r--
EH_FRAME off    0x000000000004a070 vaddr 0x000000000004a070 paddr 0x000000000004a070 align 2**2
         filesz 0x0000000000000a8c memsz 0x0000000000000a8c flags r--
   STACK off    0x0000000000000000 vaddr 0x0000000000000000 paddr 0x0000000000000000 align 2**4
         filesz 0x0000000000000000 memsz 0x0000000000000000 flags rw-
   RELRO off    0x000000000004ff28 vaddr 0x000000000024ff28 paddr 0x000000000024ff28 align 2**0
         filesz 0x00000000000030d8 memsz 0x00000000000030d8 flags r--

Dynamischer Abschnitt:
  NEEDED               libQt5Widgets.so.5
  NEEDED               libQt5Gui.so.5
  NEEDED               libQt5Sql.so.5
  NEEDED               libQt5Core.so.5
  NEEDED               libstdc++.so.6
  NEEDED               libgcc_s.so.1
  NEEDED               libc.so.6
  INIT                 0x000000000000f8c8
  FINI                 0x0000000000036c04
  INIT_ARRAY           0x000000000024ff28
  INIT_ARRAYSZ         0x0000000000000030
  FINI_ARRAY           0x000000000024ff58
  FINI_ARRAYSZ         0x0000000000000008
  GNU_HASH             0x0000000000000298
  STRTAB               0x0000000000003058
  SYMTAB               0x00000000000002e0
  STRSZ                0x00000000000040c7
  SYMENT               0x0000000000000018
  DEBUG                0x0000000000000000
  PLTGOT               0x0000000000252500
  PLTRELSZ             0x0000000000001ef0
  PLTREL               0x0000000000000007
  JMPREL               0x000000000000d9d8
  RELA                 0x0000000000007630
  RELASZ               0x00000000000063a8
  RELAENT              0x0000000000000018
  FLAGS                0x0000000000000008
  FLAGS_1              0x0000000008000001
  VERNEED              0x00000000000074f0
  VERNEEDNUM           0x0000000000000007
  VERSYM               0x0000000000007120
  RELACOUNT            0x00000000000000e5

Versionsreferenzen:
  von libgcc_s.so.1 benötigt:
    0x0b792650 0x00 11 GCC_3.0
  von libc.so.6 benötigt:
    0x06969194 0x00 12 GLIBC_2.14
    0x09691a75 0x00 10 GLIBC_2.2.5
    0x0d696914 0x00 09 GLIBC_2.4
  von libstdc++.so.6 benötigt:
    0x0297f871 0x00 14 GLIBCXX_3.4.21
    0x08922974 0x00 13 GLIBCXX_3.4
    0x0bafd179 0x00 07 CXXABI_1.3.9
    0x056bafd3 0x00 06 CXXABI_1.3
  von libQt5Sql.so.5 benötigt:
    0x00058a25 0x00 05 Qt_5
  von libQt5Gui.so.5 benötigt:
    0x00058a25 0x00 04 Qt_5
  von libQt5Core.so.5 benötigt:
    0x058a2819 0x00 08 Qt_5.9
    0x00058a25 0x00 03 Qt_5
  von libQt5Widgets.so.5 benötigt:

# ich habe es erfolglos mit
- strip DBManager
# und
- chrpath -d DBManager
# versucht, hat sich nichts geändert.

# qt statisch builden:
mkdir /media/Austausch/Qt
cd /media/Austausch/Qt
git clone git://code.qt.io/qt/qt5.git
cd qt5
perl init-repository
export LLVM_INSTALL_DIR=/usr/llvm
../qt5/.configure -static -prefix /media/Austausch/Qt/qt5/qtbase -developer-build -opensource -nomake examples -nomake tests
make

# weitere vorlage war diese hier. damit habe ich die Qt sources das 2. mal gebaut
https://retifrav.github.io/blog/2018/02/17/build-qt-statically/

./configure -static -prefix /media/Austausch/Qt/5.15_static_new -release -ltcg -optimize-size -no-pch -confirm-license -opengl -system-xcb -qt-libpng -qt-libjpeg -qt-zlib -qt-pcre -gtk -system-freetype -system-harfbuzz -pulseaudio -alsa
./configure -static -prefix /media/Austausch/Qt/5.15_static -release -ltcg -optimize-size -no-pch -confirm-license -opengl -qt-libpng -qt-libjpeg -qt-zlib -qt-pcre -gtk -system-freetype -system-harfbuzz -pulseaudio -alsa -sql-mysql -sqlite -sql-db2 -sql-ibase -sql-oci -sql-odbc -sql-psql -sql-sqlite2 -sql-sqlite -sql-tds

#letzte ausführung:
./configure -static -prefix /media/Austausch/Qt/5.15_static -release -ltcg -optimize-size -no-pch -confirm-license -opengl -qt-libpng -qt-libjpeg -qt-zlib -qt-pcre -gtk -qt-freetype

# dev libs installieren:
sudo apt install libfreetype6-dev libclang-dev gperf bison bison-doc bisonc++ bisonc++-doc flexc flex pkg-config-x86-64-linux-gnu pkg-config-i686-linux-gnu

# klappte alles nicht, verfolge jetzt folgenden plan:
https://wohlsoft.ru/pgewiki/Building_static_Qt_5

sudo apt install libnss3-dev

# ablauf:
make clean
rm -rf config.cache
./configure -static -prefix /media/Austausch/Qt/5.15_static -release -ltcg -optimize-size -no-pch -confirm-license -opengl -qt-libpng -qt-libjpeg -qt-zlib -qt-pcre -gtk -qt-freetype
PATH=/media/Austausch/Qt/qt5/qtbase/bin:$PATH
export PATH
qmake -config release
make

# hilfreich:
https://wiki.qt.io/Building_Qt_5_from_Git
#ablauf:
# vorbereitung:
sudo apt-get build-dep qt5-default
sudo apt-get install libxcb-xinerama0-dev

sudo apt-get install build-essential perl python git
sudo apt-get install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev
sudo apt-get install flex bison gperf libicu-dev libxslt-dev ruby
sudo apt-get install libssl-dev libxcursor-dev libxcomposite-dev libxdamage-dev libxrandr-dev libdbus-1-dev libfontconfig1-dev libcap-dev libxtst-dev libpulse-dev libudev-dev libpci-dev libnss3-dev libasound2-dev libxss-dev libegl1-mesa-dev gperf bison
sudo apt-get install libbz2-dev libgcrypt11-dev libdrm-dev libcups2-dev libatkmm-1.6-dev
sudo apt-get install libasound2-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
sudo apt install libclang-6.0-dev llvm-6.0

# auschecken:
git clone git://code.qt.io/qt/qt5.git
cd qt5
git checkout 5.15
(git submodule update --init --recursive)
perl init-repository

# konfigurieren und erstellen:
export LLVM_INSTALL_DIR=/usr/llvm
#../qt5/configure -developer-build -opensource -nomake examples -nomake tests
./configure -static -prefix /media/Austausch/Qt/5.15_static -ltcg -optimize-size -no-pch -confirm-license -opengl -qt-libpng -qt-libjpeg -qt-zlib -qt-pcre -gtk -qt-freetype -pulseaudio -alsa -opensource

make -j4

# versuche gerade noch einen anderen build mit:
# problem war, dass wayland und xcb probleme gemacht haben. mal wieder.
./configure -prefix /media/Austausch/Qt/5.15_static -sysconfdir /etc/xdg -confirm-license -opensource -static -dbus-linked -openssl-linked -system-harfbuzz -system-sqlite -nomake examples -no-rpath -skip qtwebengine

# die app wird erstmal erstellt und ist jetzt 15MB groß statt 400Kb
# es kommt aber :
qt.qpa.plugin: Could not find the Qt platform plugin "xcb" in ""
# mit
export QT_DEBUG_PLUGINS=1
# keine ahnung ob das vorher auch schon kam, jetzt steht da aber jedenfalls noch
QFactoryLoader::QFactoryLoader() ignoring "org.qt-project.Qt.QPA.QPlatformIntegrationFactoryInterface.5.3" since plugins are disabled in static builds

##########################
# APP erstellen
##########################
make clean
PATH=/media/Austausch/Qt/5.15_static/bin:$PATH
export PATH
qmake -config release
make

##########################
# SQLDRIVER erstellen
##########################
cd /media/Austausch/Qt/qt5/qtbase/src/plugins/sqldrivers/mysql
make clean
qmake -- MYSQL_INCDIR=/usr/include MYSQL_LIBDIR=/usr/lib
make
make install


# ich habe jetzt linuxdeployqt auf docker installiert. scheint erstmal an sich zu laufen:
Install Docker:
wget https://raw.githubusercontent.com/J-Rios/BASH-Scripts/master/install/install_docker
sudo chmod +x install_docker
sudo ./install_docker
Get Ubuntu 14.04 Docker base image:
sudo docker pull ubuntu:trusty
Launch a container of the downloaded Ubuntu image:
sudo docker run -d --name dockerdeployqt -it ubuntu:trusty
Enter the Container and get all required elements:
sudo docker exec -it dockerdeployqt bash
cd /root
apt-get update
apt-get -y install wget
apt-get -y install libglib2.0-0
Install QT5:
wget https://raw.githubusercontent.com/J-Rios/BASH-Scripts/master/install/install_qt5
chmod +x install_qt5
./install_qt5
rm -f ./install_qt5
Get and setup linuxdeployqt tool:
wget https://github.com/probonopd/linuxdeployqt/releases/download/6/linuxdeployqt-6-x86_64.AppImage
chmod +x linuxdeployqt-6-x86_64.AppImage
./linuxdeployqt-6-x86_64.AppImage --appimage-extract
mv squashfs-root deployqt
ln -s /root/deployqt/AppRun /usr/local/sbin/linuxdeployqt
rm -f linuxdeployqt-6-x86_64.AppImage
Check that linuxdeployqt is working:
linuxdeployqt
Exit the Container ad stop it (everything is ready to be used):
exit
sudo docker stop dockerdeployqt

# ist von: https://gist.github.com/J-Rios/e4e6575624af2068c8927541213c5cd1

# auf dem container habe ich installiert:
apt update -y && apt install -y git build-essential wget doxygen pkg-config qt5-default qtbase5-dev qtdeclarative5-dev qtdeclarative5-dev-tools qttools5-dev-tools

# Translations erzeugen:
# folgende einträge in DBManager.pro einfügen:
TRANSLATIONS += translations/dbManager_de.ts \
    translations/dbManager_en.ts

CODECFORSRC     = UTF-8
# ausführen:
lupdate DBManager.pro
# qm datei erstellen:
lrelease DBManager.pro

