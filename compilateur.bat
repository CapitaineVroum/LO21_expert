@echo off
echo ==========================================
echo      COMPILATION SYSTEME EXPERT (MODULAIRE)
echo ==========================================

:: 1. Création du dossier de sortie si besoin (dans app/)
if not exist app mkdir app

:: 2. La commande de compilation adaptée à ton architecture
:: -I... : On indique où sont les .h (dans le dossier de la lib)
:: On compile le main (dans app/) ET les sources de la lib (dans LO21.../src/)

gcc -Wall ^
 -ILO21_library_real_one_this_time/include ^
 app/main.c ^
 LO21_library_real_one_this_time/src/proposition.c ^
 LO21_library_real_one_this_time/src/regle.c ^
 LO21_library_real_one_this_time/src/inference.c ^
 LO21_library_real_one_this_time/src/liste.c ^
 LO21_library_real_one_this_time/src/io.c ^
 LO21_library_real_one_this_time/src/parser.c ^
 -o app/projet_expert.exe

:: 3. Vérification des erreurs
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo [ERREUR] La compilation a echoue !
    echo Verifie les messages ci-dessus.
    pause
    exit /b
)

echo.
echo [SUCCES] Compilation reussie.
echo Lancement du programme...
echo ========================================================
echo.

:: 4. Lancement
.\app\projet_expert.exe

echo.
pause