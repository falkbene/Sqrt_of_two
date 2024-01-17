cmake_minimum_required(VERSION 3.25)
project(team105)

set(CMAKE_CXX_STANDARD 17)

include_directories(Implementierung)

add_executable(team105
        Implementierung/Expo.h Implementierung/expo.c Implementierung/divisionTests.c Implementierung/main.c Implementierung/longKomma.c Implementierung/LongKomma.h Implementierung/intArr.c Implementierung/IntArr.h Implementierung/bignum.c Implementierung/Bignum.h)
