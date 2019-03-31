#pio run --target clean

SOURCE_FILE="src/sketch/Serial1602ShieldSystemUI/Serial1602ShieldSystemUI.ino"

sed -i "s/#define VERSION .*/#define VERSION \"1-0-0-0\"/" $SOURCE_FILE
