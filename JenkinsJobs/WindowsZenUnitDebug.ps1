$ErrorActionPreference = "Stop"
python ZenUnitPy\ZenUnitPy\RunTestsWithCoverageAndLinting.py
python ZenUnitPy\ZenUnitPy\BuildZenUnit.py --generator="Visual Studio 15 2017 Win64" --buildType=Debug --definitions="" --installDirectory=C:\install
