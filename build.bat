if not exist ".\build" mkdir .\build
cd build
del /S /F /Q .\**
cmake ..
msbuild ".\air-type.sln"
xcopy /s /Y /Q Debug\air-type.exe ..\