'''
 *  $Id: SConstruct, 2014-11-04 01:54:23+01:00 DAMPE $
 *----------------------------------------------------------
 *  This file is generated by dmpsw-expand
 *----------------------------------------------------------
'''

import os

target='DmpAnalysis'

print "---> compiling: %s\n" % target
#--------------------------------------------------------------------
# environment
env = Environment(ENV = os.environ)
env.ParseConfig("dmpsw-config --include --libs")
env.ParseConfig("python-config --include --libs")
env.ParseConfig("root-config --cflags --libs")
#env.ParseConfig("clhep-config --include --libs")
#env.PrependENVPath('PATH', os.environ['G4INSTALL']+'/../../../bin')    # 2 lines for G4
#env.ParseConfig("geant4-config --cflags --libs")
#env.MergeFlags('-fopenmp')

#-------------------------------------------------------------------
# include dir. and external libs
env.Prepend(CPPPATH=['./include'])
#env.Append(LIBS=['YourEventModule'])   # link your event module, only for algorithm

#-------------------------------------------------------------------
# main build
prefix=os.environ['DMPSWWORK']
goal=env.SharedLibrary(target,Glob('./src/*.cc'))
Default(env.InstallAs(prefix+'/lib/lib'+target+".so",goal))
if env['PLATFORM'] == 'darwin':    #MacOS
    Default(env.InstallAs(prefix+'/lib/lib'+target+".dylib",goal))