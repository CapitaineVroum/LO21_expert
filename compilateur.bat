@echo off
echo ==========================================
echo      COMPILATION DU PROJET LO21
echo ==========================================

if not exist build mkdir build

cd build

echo Configuration...
cmake -G "MinGW Makefiles" ..
if %errorlevel% neq 0 goto ERROR

echo Compilation en cours...
cmake --build .
if %errorlevel% neq 0 goto ERROR

echo.
echo ==========================================
echo      LANCEMENT DU SYSTEME EXPERT
echo ==========================================
echo.

if exist app\Debug\mon_systeme_expert.exe (
    .\app\Debug\mon_systeme_expert.exe
) else (
    .\app\mon_systeme_expert.exe
)

goto END

:ERROR
echo.
echo !!!!!!! ERREUR LORS DE LA COMPILATION !!!!!!!
echo Verifie ton code.

:END
echo.
pause