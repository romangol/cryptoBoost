import os
env = Environment(ENV = os.environ)

File_list = ['./blockcipher/sm4.cpp', './hash/sm3.cpp', './bn/bn_boost.cpp', './pubkey/ecc.cpp', './pubkey/RSA.cpp', './tester.cpp']

#Lib_list = ['kernel32.lib', 'user32.lib', 'winmm.lib', 'wsock32.lib']
#env.Append( LIBS = Lib_list )

#Inc_list = ['./src/']
#env.Append( CPPPATH = Inc_list )

Def_list = ['WIN32','_WINDOWS', '_CRT_SECURE_NO_WARNINGS', 'NDEBUG']
ccflags_str = '/EHsc /nologo /W3 /O2 /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /FD /c'

# lflags_str = 'kernel32.lib user32.lib wsock32.lib winmm.lib /nologo /subsystem:console /incremental:yes /machine:I386 /out:nc.exe'

env = Environment( ENV = os.environ )


env.Append( CPPDEFINES = Def_list )
env.Append( CCFLAGS = ccflags_str )

env.Program( 'crypt', File_list )

