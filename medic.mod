+ PLATFORM:mac medic 1.0.0 ..
PYTHONPATH+:=py
DYLD_LIBRARY_PATH+:=lib
MEDIC_TESTER_PATH+:=plugins/Tester
MEDIC_KARTE_PATH+:=plugins/Karte

+ PLATFORM:win64 medic 1.0.0 ..
PYTHONPATH+:=py
PATH+:=bin
MEDIC_TESTER_PATH+:=plugins/Tester
MEDIC_KARTE_PATH+:=plugins/Karte

+ PLATFORM:linux medic 1.0.0 ..
PYTHONPATH+:=py
LD_LIBRARY_PATH+:=lib
MEDIC_TESTER_PATH+:=plugins/Tester
MEDIC_KARTE_PATH+:=plugins/Karte