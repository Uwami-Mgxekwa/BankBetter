@echo off
setlocal enabledelayedexpansion

REM Navigate to your repo folder
cd /d "C:\Users\ZiloTech\Desktop\BankBetter\test.txt"

REM Define commit messages array (add/remove as you like)
set "msgs[0]=fix layout issue on dashboard"
set "msgs[1]=update login screen UI"
set "msgs[2]=refactor authentication logic"
set "msgs[3]=improve button spacing on mobile"
set "msgs[4]=resolve merge conflict in header"
set "msgs[5]=add loading spinner to form"
set "msgs[6]=clean up unused imports"
set "msgs[7]=patch null pointer in user service"
set "msgs[8]=update README with setup steps"
set "msgs[9]=adjust font size for readability"
set "msgs[10]=fix broken link in navbar"
set "msgs[11]=modify register page layout"
set "msgs[12]=remove deprecated API calls"
set "msgs[13]=improve error handling in login"
set "msgs[14]=update color scheme to match brand"
set "msgs[15]=fix typo in welcome message"
set "msgs[16]=add input validation to signup form"
set "msgs[17]=optimize image loading performance"
set "msgs[18]=fix padding issue on mobile view"
set "msgs[19]=update dependencies to latest version"
set "msgs[20]=refactor sidebar navigation"
set "msgs[21]=fix session timeout bug"
set "msgs[22]=improve password strength checker"
set "msgs[23]=update footer links"
set "msgs[24]=add dark mode toggle"
set "msgs[25]=fix alignment issue in card grid"
set "msgs[26]=modify profile page header"
set "msgs[27]=resolve CORS issue in API calls"
set "msgs[28]=update env variable references"
set "msgs[29]=fix 404 redirect on logout"

REM Loop 300 times
for /l %%i in (1,1,986) do (

    REM Pick a message using modulo 30
    set /a idx=%%i %% 30
    set "msg=!msgs[!idx!]!"

    REM Append a small change to test.txt
    echo update %%i >> test.txt

    REM Stage and commit with a real-looking message
    git add test.txt
    git commit -m "!msg!"

    REM Show progress
    echo [Brelinx] Commit %%i/986 -- !msg!
)

REM Push all commits at once
git push origin main
pause