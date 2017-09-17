import os
import platform
import sys
sys.path.append('ZenUnitPy') # Jenkins
sys.path.append('../ZenUnitPy') # Jenkins
from ZenUnitPy import Util, BuildZenUnit

def main(args):
   if len(args) != 4:
      Util.print_and_exit(
'Invalid args: ' + ' '.join(args) + '\n' +
"""Usage: python3 BuildZenUnitHelloWorld.py --generator=<CMakeGenerator> --buildType=<CMakeBuildType> --definitions=<QuotedSpaceSeparatedCMakeDefinitions>""", 1)
   else:
      cmakeGenerator = args[1]
      cmakeBuildType = args[2]
      cmakeDefinitions = args[3]
      casefoldedPlatformSystem = platform.system().casefold()
      if casefoldedPlatformSystem == 'linux':
         BuildZenUnit.linux_cmake_and_build(cmakeGenerator, cmakeBuildType, cmakeDefinitions)
         BuildZenUnit.linux_run_zenunit_tests('ZenUnitHelloWorldTests')
         os.chdir('..')
      else:
         BuildZenUnit.windows_cmake_and_build(cmakeGenerator, cmakeBuildType, cmakeDefinitions)
         BuildZenUnit.windows_run_zenunit_tests(cmakeBuildType, 'ZenUnitHelloWorldTests')

if __name__ == "__main__": # pragma nocover
   main(sys.argv)
