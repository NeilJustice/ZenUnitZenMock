python.exe ZenUnitPy\ZenUnitPy\RunZenUnitPyTestsWithCoverageAndLinting.py
if ($LastExitCode -ne 0) { exit $LastExitCode }
python.exe ZenUnitPy\ZenUnitPy\BuildZenUnitOrZenMock.py --ZenUnitOrZenMock=ZenUnit --generator="Visual Studio 15 2017 Win64" --buildType=Debug --definitions=""