rem This will create a folder with everything needed to zip up and put on the website
xcopy ..\client deploy-final\client /S /D /I /EXCLUDE:excludes-final.txt
xcopy ..\rules.txt deploy-final\ /D
xcopy ..\aitournament.ppt deploy-final\ /D
xcopy ..\"The story so far.txt" deploy-final\ /D